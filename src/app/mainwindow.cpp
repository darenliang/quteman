#include <QString>
#include <QDebug>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/util.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Init json model
    jsonModel = new QJsonModel;
    ui->jsonView->setModel(jsonModel);

    // Add right most status message
    statusMessage = new QLabel;
    statusMessage->setAlignment(Qt::AlignRight);
    ui->statusbar->addPermanentWidget(statusMessage, 1);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Quit action
void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}

// About action
void MainWindow::on_actionAbout_triggered() {
    about.exec();
}

std::optional<cpr::Response> MainWindow::makeRequest() {
    auto url = cpr::Url(ui->addressBar->text().toStdString());
    auto index = ui->requestType->currentIndex();

    cpr::Session session;
    session.SetUrl(url);

    // Authorization input
    auto username = ui->authUsernameInput->text().toStdString();
    auto password = ui->authPasswordInput->text().toStdString();

    // If username and password are provided
    if (!username.empty() && !password.empty()) {
        session.SetAuth(
                cpr::Authentication(
                        std::move(username),
                        std::move(password)
                )
        );
    }

    // Params input
    auto paramsInput = ui->paramsInput->toPlainText();
    auto params = Util::parseKeyValue(paramsInput);

    if (!params.empty()) {
        auto cprparams = cpr::Parameters();
        auto curlholder = cpr::CurlHolder();
        for (auto[key, value]:params) {
            cprparams.AddParameter(
                    cpr::Parameter(
                            std::move(key),
                            std::move(value)
                    ),
                    curlholder
            );
        }
        session.SetParameters(cprparams);
    }

    // Headers input
    auto headersInput = ui->headersInput->toPlainText();
    auto headers = Util::parseKeyValue(headersInput);

    if (!headers.empty()) {
        auto cprheader = cpr::Header();
        for (const auto &[key, value]:params) {
            cprheader[key] = value;
        }
        session.SetHeader(cprheader);
    }

    // Body input
    auto bodyInput = ui->bodyInput->toPlainText().toStdString();

    if (!bodyInput.empty()) {
        session.SetBody(cpr::Body(bodyInput));
    }

    switch (index) {
        case GET:
            return session.Get();
        case POST:
            return session.Post();
        case PUT:
            return session.Put();
        case PATCH:
            return session.Patch();
        case DELETE:
            return session.Delete();
        default:
            qDebug() << "invalid request type: " << index;
            return std::nullopt;
    }
}

template<typename T>
void MainWindow::updateStatusBar(
        const T &status,
        double time, double bytes
) {
    statusMessage->setText(
            QString("Status: %1    Time: %2 ms    Size: %3")
                    .arg(status)
                    .arg(static_cast<int>(time * 1000))
                    .arg(QLocale::system().formattedDataSize(bytes))
    );
}

// Submit button
void MainWindow::on_submitButton_clicked() {
    auto res = makeRequest();

    // If null operation
    if (!res) {
        return;
    }

    auto resp = res.value();

    // Set raw text
    ui->textBrowser->setText(QString::fromStdString(resp.text));

    // Set response headers
    QString headers;
    for (auto const &[key, value]:resp.header) {
        headers.append(
                QString("%1: %2\n")
                        .arg(key.c_str())
                        .arg(value.c_str())
        );
    }
    ui->headersBrowser->setText(headers);

    if (resp.error) {
        qDebug() << "request error: " << resp.error.message.c_str();

        updateStatusBar<QString>(
                "No Connection",
                resp.elapsed,
                resp.downloaded_bytes
        );
        return;
    }

    if (!jsonModel->loadJson(QByteArray::fromStdString(resp.text))) {
        qDebug() << "json failed on url: " << resp.url.c_str();

        updateStatusBar<QString>(
                "Invalid Json",
                resp.elapsed,
                resp.downloaded_bytes
        );
        return;
    }

    // Expand view by default
    ui->jsonView->expandAll();

    updateStatusBar<std::int32_t>(
            resp.status_code,
            resp.elapsed,
            resp.downloaded_bytes
    );
}