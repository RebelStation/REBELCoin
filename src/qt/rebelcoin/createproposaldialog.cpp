// Copyright (c) 2021 The PIVX developers
// Copyright (c) 2021 The REBELCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#include "qt/rebelcoin/createproposaldialog.h"
#include "qt/rebelcoin/forms/ui_createproposaldialog.h"

#include "qt/rebelcoin/contactsdropdown.h"
#include "qt/rebelcoin/governancemodel.h"
#include "qt/rebelcoin/pwidget.h"
#include "qt/rebelcoin/qtutils.h"
#include "qt/rebelcoin/snackbar.h"

void initPageIndexBtn(QPushButton* btn)
{
    QSize BUTTON_SIZE = QSize(22, 22);
    setCssProperty(btn, "ic-step-confirm");
    btn->setMinimumSize(BUTTON_SIZE);
    btn->setMaximumSize(BUTTON_SIZE);
    btn->move(0, 0);
    btn->show();
    btn->raise();
    btn->setVisible(false);
}

CreateProposalDialog::CreateProposalDialog(REBELCoinGUI* parent, GovernanceModel* _govModel, WalletModel* _walletModel) :
    QDialog(parent),
    ui(new Ui::CreateProposalDialog),
    govModel(_govModel),
    walletModel(_walletModel),
    icConfirm1(new QPushButton()),
    icConfirm2(new QPushButton()),
    icConfirm3(new QPushButton())
{
    ui->setupUi(this);
    this->setStyleSheet(parent->styleSheet());

    setCssProperty(ui->frame, "container-dialog");
    ui->frame->setContentsMargins(10, 10, 10, 10);
    setCssProperty({ui->labelLine1, ui->labelLine2}, "line-purple");
    setCssProperty({ui->groupBoxName, ui->groupContainer}, "container-border");
    setCssProperty({ui->pushNumber1, ui->pushNumber2, ui->pushNumber3}, "btn-number-check");
    setCssProperty({ui->pushName1, ui->pushName2, ui->pushName3}, "btn-name-check");

    // Pages setup
    setupPageOne();
    setupPageTwo();
    setupPageThree();

    // Confirm icons
    ui->stackedIcon1->addWidget(icConfirm1);
    ui->stackedIcon2->addWidget(icConfirm2);
    ui->stackedIcon3->addWidget(icConfirm3);
    initPageIndexBtn(icConfirm1);
    initPageIndexBtn(icConfirm2);
    initPageIndexBtn(icConfirm3);

    // Connect btns
    setCssProperty(ui->btnNext, "btn-primary");
    ui->btnNext->setText(tr("NEXT"));
    setCssProperty(ui->btnBack, "btn-dialog-cancel");
    ui->btnBack->setVisible(false);
    ui->btnBack->setText(tr("BACK"));
    setCssProperty(ui->pushButtonSkip, "ic-close");

    connect(ui->pushButtonSkip, &QPushButton::clicked, this, &CreateProposalDialog::close);
    connect(ui->btnNext, &QPushButton::clicked, this, &CreateProposalDialog::onNextClicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &CreateProposalDialog::onBackClicked);
}

void setEditBoxStyle(QLabel* label, QLineEdit* lineEdit, const QString& placeholderText)
{
    setCssProperty(label, "text-title");
    lineEdit->setPlaceholderText(placeholderText);
    setCssProperty(lineEdit, "edit-primary");
    lineEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    setShadow(lineEdit);
}

void CreateProposalDialog::setupPageOne()
{
    setCssProperty(ui->labelTitle1, "text-title-dialog");
    setCssProperty(ui->labelMessage1b, "dialog-proposal-message");
    setEditBoxStyle(ui->labelName, ui->lineEditPropName, "e.g Best proposal ever!");
    setEditBoxStyle(ui->labelURL, ui->lineEditURL, "e.g https://forum.rebelcoin/proposals/best_proposal_ever");

    connect(ui->lineEditPropName, &QLineEdit::textChanged, this, &CreateProposalDialog::propNameChanged);
    connect(ui->lineEditURL, &QLineEdit::textChanged, this, &CreateProposalDialog::propUrlChanged);
}

void CreateProposalDialog::setupPageTwo()
{
    setCssProperty(ui->labelTitleDest, "text-title-dialog");
    setCssProperty(ui->labelMessageDest, "dialog-proposal-message");
    setEditBoxStyle(ui->labelAmount, ui->lineEditAmount, "e.g 500 RBL");

    setEditBoxStyle(ui->labelAddress, ui->lineEditAddress, "e.g D...something..");
    setCssProperty(ui->lineEditAddress, "edit-primary-multi-book");
    actAddrList = ui->lineEditAddress->addAction(QIcon("://ic-contact-arrow-down"), QLineEdit::TrailingPosition);
    ui->lineEditAmount->setValidator(new QIntValidator(1, govModel->getMaxAvailableBudgetAmount() / 100000000, this));
    setCssProperty(ui->lineEditMonths, "btn-spin-box");
    setShadow(ui->lineEditMonths);
    ui->lineEditMonths->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(ui->lineEditMonths, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
        &CreateProposalDialog::monthsEditDeselect, Qt::QueuedConnection);
    connect(ui->lineEditMonths->findChild<QLineEdit*>(), &QLineEdit::cursorPositionChanged,
        this, &CreateProposalDialog::monthsEditDeselect, Qt::QueuedConnection);

    connect(ui->lineEditAmount, &QLineEdit::textChanged, this, &CreateProposalDialog::propAmountChanged);
    connect(ui->lineEditAddress, &QLineEdit::textChanged, this, &CreateProposalDialog::propaddressChanged);
    connect(actAddrList, &QAction::triggered, this, &CreateProposalDialog::onAddrListClicked);
}

void CreateProposalDialog::monthsEditDeselect(int i)
{
    ui->lineEditMonths->findChild<QLineEdit*>()->deselect();
    ui->lineEditMonths->clearFocus();
}

void CreateProposalDialog::setupPageThree()
{
    setCssProperty(ui->labelTitle3, "text-title-dialog");
    ui->stackedWidget->setCurrentIndex(pos);
    setCssProperty({ui->labelResultNameTitle,
                       ui->labelResultAmountTitle,
                       ui->labelResultAddressTitle,
                       ui->labelResultMonthsTitle,
                       ui->labelResultUrlTitle},
        "text-title");
    setCssProperty({ui->labelResultName,
                       ui->labelResultName,
                       ui->labelResultAmount,
                       ui->labelResultAddress,
                       ui->labelResultMonths,
                       ui->labelResultUrl},
        "text-body1-dialog");
}

void CreateProposalDialog::propNameChanged(const QString& newText)
{
    bool isValid = !newText.isEmpty() && IsValidUTF8(newText.toStdString());
    setCssEditLine(ui->lineEditPropName, isValid, true);
}

void CreateProposalDialog::propUrlChanged(const QString& newText)
{
    setCssEditLine(ui->lineEditURL, govModel->validatePropURL(newText).getRes(), true);
}

void CreateProposalDialog::propAmountChanged(const QString& newText)
{
    CAmount amt = newText.toDouble() * COIN;
    setCssEditLine(ui->lineEditAmount, govModel->validatePropAmount(amt).getRes(), true);
}

bool CreateProposalDialog::propaddressChanged(const QString& str)
{
    if (!str.isEmpty()) {
        QString trimmedStr = str.trimmed();
        bool isShielded = false;
        const bool valid = walletModel->validateAddress(trimmedStr, false, isShielded) && !isShielded;
        setCssEditLine(ui->lineEditAddress, valid, true);
        return valid;
    }
    setCssEditLine(ui->lineEditAddress, true, true);
    return false;
}

bool CreateProposalDialog::validatePageOne()
{
    QString propName = ui->lineEditPropName->text();
    if (propName.isEmpty()) {
        setCssEditLine(ui->lineEditPropName, false, true);
        inform(tr("Proposal name cannot be empty"));
        return false;
    }

    // For now, only accept UTF8 valid strings.
    if (!IsValidUTF8(propName.toStdString())) {
        setCssEditLine(ui->lineEditPropName, false, true);
        inform(tr("Proposal name cannot contain non UTF-8 characters"));
        return false;
    }

    QByteArray propbytes = propName.toUtf8();
    int proplength = propbytes.size();

    if (proplength > 20) {
        inform(tr("Proposal name should max 20 character"));
        return false;
    }
    QString propurl = ui->lineEditURL->text();
    QByteArray propurlbytes = propurl.toUtf8();
    int propurllength = propurlbytes.size();
    if (propurllength > 64) {
        inform(tr("Proposal url should max 64 character"));
        return false;
    }


    auto res = govModel->validatePropURL(ui->lineEditURL->text());
    if (!res) inform(QString::fromStdString(res.getError()));
    return res.getRes();
}

bool CreateProposalDialog::validatePageTwo()
{
    // Amount validation
    auto opRes = govModel->validatePropAmount(ui->lineEditAmount->text().toInt());
    if (!opRes) {
        inform(QString::fromStdString(opRes.getError()));
        return false;
    }

    // Payments count validation
    opRes = govModel->validatePropPaymentCount(ui->lineEditMonths->value());
    if (!opRes) {
        inform(QString::fromStdString(opRes.getError()));
        return false;
    }

    if (!propaddressChanged(ui->lineEditAddress->text())) {
        inform(tr("Invalid payment address"));
        return false;
    }

    return true;
}

void CreateProposalDialog::loadSummary()
{
    ui->labelResultName->setText(ui->lineEditPropName->text());
    ui->labelResultUrl->setText(ui->lineEditURL->text());
    ui->labelResultAmount->setText(GUIUtil::formatBalance(ui->lineEditAmount->text().toInt() * COIN));
    ui->labelResultMonths->setText(QString::number(ui->lineEditMonths->value()));
    ui->labelResultAddress->setText(ui->lineEditAddress->text());
    ui->labelResultUrl->setText(ui->lineEditURL->text());
}

void CreateProposalDialog::sendProposal()
{
    int months = ui->lineEditMonths->value();
    CAmount amount = ui->lineEditAmount->text().toInt() * COIN;
    auto opRes = govModel->createProposal(
        ui->lineEditPropName->text().toStdString(),
        ui->lineEditURL->text().toStdString(),
        months,
        amount,
        ui->lineEditAddress->text().toStdString());
    if (!opRes) {
        inform(QString::fromStdString(opRes.getError()));
        return;
    }
    accept();
}

void CreateProposalDialog::onNextClicked()
{
    int nextPos = pos + 1;
    switch (pos) {
    case 0: {
        if (!validatePageOne()) return;
        ui->stackedWidget->setCurrentIndex(nextPos);
        ui->pushNumber2->setChecked(true);
        ui->pushName3->setChecked(false);
        ui->pushName2->setChecked(true);
        ui->pushName1->setChecked(true);
        icConfirm1->setVisible(true);
        ui->btnBack->setVisible(true);
        break;
    }
    case 1: {
        if (!validatePageTwo()) return;
        loadSummary();
        ui->stackedWidget->setCurrentIndex(nextPos);
        ui->pushNumber3->setChecked(true);
        ui->pushName3->setChecked(true);
        ui->pushName2->setChecked(true);
        ui->pushName1->setChecked(true);
        icConfirm2->setVisible(true);
        ui->btnNext->setText("Send");
        break;
    }
    case 2: {
        sendProposal();
    }
    }
    pos = nextPos;
}

void CreateProposalDialog::onBackClicked()
{
    if (pos == 0) return;
    pos--;
    switch (pos) {
    case 0: {
        ui->stackedWidget->setCurrentIndex(pos);
        ui->pushNumber1->setChecked(true);
        ui->pushNumber3->setChecked(false);
        ui->pushNumber2->setChecked(false);
        ui->pushName3->setChecked(false);
        ui->pushName2->setChecked(false);
        ui->pushName1->setChecked(true);
        icConfirm1->setVisible(false);
        ui->btnBack->setVisible(false);
        break;
    }
    case 1: {
        ui->stackedWidget->setCurrentIndex(pos);
        ui->pushNumber2->setChecked(true);
        ui->pushNumber3->setChecked(false);
        ui->pushName3->setChecked(false);
        ui->pushName2->setChecked(true);
        ui->pushName1->setChecked(true);
        icConfirm2->setVisible(false);
        ui->btnNext->setText("Next");
        break;
    }
    }
}

void CreateProposalDialog::onAddrListClicked()
{
    int addrSize = walletModel->getAddressTableModel()->sizeSend() +
                   walletModel->getAddressTableModel()->sizeRecv();
    if (addrSize == 0) {
        inform(tr("No contacts available, you can go to the contacts screen and add some there!"));
        return;
    }

    int rowHeight = ui->lineEditAddress->height();
    int height = 70 * 2 + 1; // 2 rows (70 each row).
    int width = ui->lineEditAddress->width();

    if (!menuContacts) {
        // TODO: add different row icon for contacts and own addresses.
        // TODO: add filter/search option.
        // TODO: fix bug that the last presented address isn't being showed.
        menuContacts = new ContactsDropdown(
            width,
            height,
            static_cast<REBELCoinGUI*>(parent()),
            this);
        menuContacts->setWalletModel(walletModel, {AddressTableModel::Send, AddressTableModel::Receive});
        connect(menuContacts, &ContactsDropdown::contactSelected, [this](QString address, QString label) {
            ui->lineEditAddress->setText(address);
        });
    }

    if (menuContacts->isVisible()) {
        menuContacts->hide();
        return;
    }

    menuContacts->resizeList(width, height);
    menuContacts->setStyleSheet(this->styleSheet());
    menuContacts->adjustSize();

    QPoint pos = ui->containerPage2->rect().bottomLeft();
    pos.setY(pos.y() + rowHeight * 2 - 20);
    pos.setX(pos.x() + 74); // Add widget's fixed padding manually
    menuContacts->move(pos);
    menuContacts->show();
}

void CreateProposalDialog::inform(const QString& text)
{
    if (!snackBar) snackBar = new SnackBar(nullptr, this);
    snackBar->setText(text);
    snackBar->resize(this->width(), snackBar->height());
    openDialog(snackBar, this);
}

CreateProposalDialog::~CreateProposalDialog()
{
    delete ui;
}