#include "qt/rebelcoin/governancewidget.h"
#include "qt/rebelcoin/forms/ui_governancewidget.h"

#include "qt/rebelcoin/createproposaldialog.h"
#include "qt/rebelcoin/governancemodel.h"
#include "qt/rebelcoin/mnmodel.h"
#include "qt/rebelcoin/qtutils.h"
#include "qt/rebelcoin/votedialog.h"

#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <map>

// Proposals filter
const std::vector<std::string> propFilter = {"All", "Passing", "Not Passing", "Waiting"};
GovernanceWidget::GovernanceWidget(REBELCoinGUI* parent) : PWidget(parent),ui(new Ui::governancewidget)
{
    ui->setupUi(this);
    this->setStyleSheet(parent->styleSheet());

    setCssProperty(ui->left, "container");
    ui->left->setContentsMargins(0, 20, 0, 0);
    setCssProperty(ui->right, "container-right");
    ui->right->setContentsMargins(20, 10, 20, 20);
    setCssProperty(ui->scrollArea, "container");

    /* Title */
    ui->labelTitle->setText("Governance");
    setCssProperty(ui->labelTitle, "text-title-screen");
    ui->labelSubtitle1->setText("View, follow, vote and submit network budget proposals.\nBe part of the DAO.");
    setCssProperty(ui->labelSubtitle1, "text-subtitle");
    setCssProperty(ui->pushImgEmpty, "img-empty-governance");
    setCssProperty(ui->labelEmpty, "text-empty");

    // Font
    QFont font;
    font.setPointSize(14);

    // Combo box sort
    SortEdit* lineEdit = new SortEdit(ui->comboBoxSort);
    lineEdit->setFont(font);
    lineEdit->setAlignment(Qt::AlignRight);
    initComboBox(ui->comboBoxSort, lineEdit, "btn-combo", false);

    QStandardItemModel* model = new QStandardItemModel(this);
    Delegate* delegate = new Delegate(this);
    QList<QString> values; // todo: Add sort actions
    values.append("Date");
    values.append("Value");
    values.append("Name");
    for (const auto& value : values) {
        model->appendRow(new QStandardItem(tr("Sort by: %1").arg(value)));
    }
    delegate->setValues(values);
    ui->comboBoxSort->setModel(model);
    ui->comboBoxSort->setItemDelegate(delegate);
    ui->comboBoxSort->setVisible(false);

    // Filter
    SortEdit* lineEditFilter = new SortEdit(ui->comboBoxFilter);
    lineEditFilter->setFont(font);
    initComboBox(ui->comboBoxFilter, lineEditFilter, "btn-filter", false);

    QStandardItemModel* modelFilter = new QStandardItemModel(this);
    Delegate* delegateFilter = new Delegate(this);
    QList<QString> valuesFilter;
    for (int i = 0; i < propFilter.size(); ++i) {
        QString str = QString::fromStdString(propFilter[i]);
        valuesFilter.append(str);
        auto item = new QStandardItem(tr("Filter: %1").arg(str));
        item->setData(i);
        modelFilter->appendRow(item);
    }
    delegateFilter->setValues(valuesFilter);
    ui->comboBoxFilter->setModel(modelFilter);
    ui->comboBoxFilter->setItemDelegate(delegateFilter);
    ui->comboBoxFilter->setCurrentIndex(0);
    connect(ui->comboBoxFilter, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),
        this, &GovernanceWidget::onFilterChanged);

    // Budget
    ui->labelBudget->setText("Budget Distribution");
    setCssProperty(ui->labelBudget, "btn-title-grey");
    setCssProperty(ui->labelBudgetSubTitle, "text-subtitle");
    setCssProperty(ui->labelAvailableTitle, "label-budget-text");
    setCssProperty(ui->labelAllocatedTitle, "label-budget-text");
    setCssProperty(ui->labelAvailableAmount, "label-budget-amount");
    setCssProperty(ui->labelAllocatedAmount, "label-budget-amount-allocated");
    setCssProperty(ui->iconClock, "ic-time");
    setCssProperty(ui->labelNextSuperblock, "label-budget-text");

    // Sync Warning
    ui->layoutWarning->setVisible(true);
    ui->lblWarning->setText(tr("Please wait until the node is fully synced to see the correct information"));
    setCssProperty(ui->lblWarning, "text-warning");
    setCssProperty(ui->imgWarning, "ic-warning");

    // Create proposal
    ui->btnCreateProposal->setTitleClassAndText("btn-title-grey", "Create Proposal");
    ui->btnCreateProposal->setSubTitleClassAndText("text-subtitle", "Prepare and submit a new proposal.");
    connect(ui->btnCreateProposal, &OptionButton::clicked, this, &GovernanceWidget::onCreatePropClicked);
    ui->emptyContainer->setVisible(false);
}

GovernanceWidget::~GovernanceWidget()
{
    delete ui;
}

void GovernanceWidget::onFilterChanged(const QString& value)
{
    int filterByType = ui->comboBoxFilter->currentIndex();
    switch (filterByType) {
    case 1:
        statusFilter = ProposalInfo::Status::PASSING;
        break;
    case 2:
        statusFilter = ProposalInfo::Status::NOT_PASSING;
        break;
    case 3:
        statusFilter = ProposalInfo::Status::WAITING_FOR_APPROVAL;
        break;
    default:
        statusFilter = nullopt;
        break;
    }
    refreshCardsGrid(true);
}

void GovernanceWidget::onVoteForPropClicked(const ProposalInfo& proposalInfo)
{
    if (!governanceModel->isTierTwoSync()) {
        inform(tr("Please wait until the node is fully synced"));
        return;
    }

    if (proposalInfo.status == ProposalInfo::Status::WAITING_FOR_APPROVAL) {
        inform(tr("Cannot vote for the proposal yet, wait until it's confirmed by the network"));
        return;
    }
    window->showHide(true);
    VoteDialog* dialog = new VoteDialog(window, governanceModel, mnModel);
    dialog->setProposal(proposalInfo);
    if (openDialogWithOpaqueBackgroundY(dialog, window, 4.5, 5)) {
        // future: make this refresh atomic, no need to refresh the entire grid.
        tryGridRefresh(true);
        inform(tr("Vote emitted successfully!"));
    }
    dialog->deleteLater();
}

void GovernanceWidget::onCreatePropClicked()
{
    window->showHide(true);
    CreateProposalDialog* dialog = new CreateProposalDialog(window, governanceModel, walletModel);
    if (openDialogWithOpaqueBackgroundY(dialog, window, 4.5, ui->left->height() < 700 ? 12 : 5)) {
        // future: make this refresh atomic, no need to refresh the entire grid.
        tryGridRefresh(true);
        inform(tr("Proposal transaction fee broadcasted!"));
    }
    dialog->deleteLater();
}

void GovernanceWidget::loadClientModel()
{
    connect(clientModel, &ClientModel::numBlocksChanged, this, &GovernanceWidget::chainHeightChanged);
}

void GovernanceWidget::chainHeightChanged(int height)
{
    if (!isVisible() || clientModel->inInitialBlockDownload()) return;
    int remainingBlocks = governanceModel->getNextSuperblockHeight() - height;
    int remainingDays = remainingBlocks / 720;
    QString text = remainingDays == 0 ? tr("Next superblock today!\n%2 blocks to go.").arg(remainingBlocks) :
                                        tr("Next superblock in %1 days.\n%2 blocks to go.").arg(remainingDays).arg(remainingBlocks);
    ui->labelNextSuperblock->setText(text);
}

void GovernanceWidget::setGovModel(GovernanceModel* _model)
{
    governanceModel = _model;
}

void GovernanceWidget::setMNModel(MNModel* _mnModel)
{
    mnModel = _mnModel;
}

void GovernanceWidget::loadWalletModel()
{
    governanceModel->setWalletModel(walletModel);
}

void GovernanceWidget::showEvent(QShowEvent* event)
{
    tryGridRefresh(true); // future: move to background worker
    if (!refreshTimer) refreshTimer = new QTimer(this);
    if (!refreshTimer->isActive()) {
        connect(refreshTimer, &QTimer::timeout, [this]() { tryGridRefresh(true); });
        refreshTimer->start(1000 * 60 * 3.5); // Try to refresh screen 3.5 minutes
    }
}

void GovernanceWidget::hideEvent(QHideEvent* event)
{
    refreshTimer->stop();
}

void GovernanceWidget::resizeEvent(QResizeEvent* event)
{
    if (!isVisible()) return;
    tryGridRefresh();
}

void GovernanceWidget::tryGridRefresh(bool force)
{
    int _propsPerRow = calculateColumnsPerRow();
    if (_propsPerRow != propsPerRow || force) {
        propsPerRow = _propsPerRow;
        refreshCardsGrid(true);

        // refresh budget distribution values
        chainHeightChanged(clientModel->getNumBlocks());
        ui->labelAllocatedAmount->setText(GUIUtil::formatBalance(governanceModel->getBudgetAllocatedAmount()));
        ui->labelAvailableAmount->setText(GUIUtil::formatBalance(governanceModel->getBudgetAvailableAmount()));
    }
}

static void setCardShadow(QWidget* edit)
{
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setColor(QColor(77, 77, 77, 30));
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(4);
    shadowEffect->setBlurRadius(6);
    edit->setGraphicsEffect(shadowEffect);
}

ProposalCard* GovernanceWidget::newCard()
{
    ProposalCard* propCard = new ProposalCard(ui->scrollAreaWidgetContents);
    connect(propCard, &ProposalCard::voteClicked, this, &GovernanceWidget::onVoteForPropClicked);
    connect(propCard, &ProposalCard::inform, this, &GovernanceWidget::inform);
    setCardShadow(propCard);
    return propCard;
}

void GovernanceWidget::showEmptyScreen(bool show)
{
    if (ui->emptyContainer->isVisible() != show) {
        ui->emptyContainer->setVisible(show);
        ui->mainContainer->setVisible(!show);
    }
}

void GovernanceWidget::refreshCardsGrid(bool forceRefresh)
{
    if (!governanceModel) return;
    if (!governanceModel->hasProposals()) {
        showEmptyScreen(true);
        return;
    }

    showEmptyScreen(false);
    if (!gridLayout) {
        gridLayout = new QGridLayout();
        gridLayout->setAlignment(Qt::AlignTop);
        gridLayout->setHorizontalSpacing(16);
        gridLayout->setVerticalSpacing(16);
        ui->scrollArea->setWidgetResizable(true);
        ui->scrollAreaWidgetContents->setLayout(gridLayout);
    }

    // Refresh grid only if needed
    if (!(forceRefresh || governanceModel->isRefreshNeeded())) return;

    std::list<ProposalInfo> props = governanceModel->getProposals(statusFilter.get_ptr());

    // Start marking all the cards
    for (ProposalCard* card : cards) {
        card->setNeedsUpdate(true);
    }

    // Refresh the card if exists or create a new one.
    int column = 0;
    int row = 0;
    for (const auto& prop : props) {
        QLayoutItem* item = gridLayout->itemAtPosition(row, column);
        ProposalCard* card{nullptr};
        if (item) {
            card = dynamic_cast<ProposalCard*>(item->widget());
            card->setNeedsUpdate(false);
        } else {
            card = newCard();
            cards.emplace_back(card);
            gridLayout->addWidget(card, row, column, 1, 1);
        }
        card->setProposal(prop);
        column++;
        if (column == propsPerRow) {
            column = 0;
            row++;
        }
    }

    // Now delete the not longer needed cards
    auto it = cards.begin();
    while (it != cards.end()) {
        ProposalCard* card = (*it);
        if (!card->isUpdateNeeded()) {
            it++;
            continue;
        }
        gridLayout->takeAt(gridLayout->indexOf(card));
        it = cards.erase(it);
        delete card;
    }
}

int GovernanceWidget::calculateColumnsPerRow()
{
    int widgetWidth = ui->left->width();
    if (widgetWidth < 785) {
        return 2;
    } else if (widgetWidth < 1100) {
        return 3;
    } else {
        return 4; // max amount of cards
    }
}

void GovernanceWidget::tierTwoSynced(bool sync)
{
    if (isSync != sync) {
        isSync = sync;
        ui->layoutWarning->setVisible(!isSync);
        if (!isVisible()) return;
        tryGridRefresh();
    }
}