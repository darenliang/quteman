#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <optional>
#include <cpr/cpr.h>

#include "src/about/about.h"
#include "lib/QJsonModel/qjsonmodel.h"

enum RequestType {
    GET = 0,
    POST = 1,
    PUT = 2,
    PATCH = 3,
    DELETE = 4
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    static void on_actionQuit_triggered();

    void on_actionAbout_triggered();

    void on_submitButton_clicked();

private:
    Ui::MainWindow *ui;
    About about;
    QJsonModel *jsonModel;
    QLabel *statusMessage;

    template<typename T>
    void updateStatusBar(const T &status, double time, double bytes);

    std::optional<cpr::Response> makeRequest();
};

#endif // MAINWINDOW_H
