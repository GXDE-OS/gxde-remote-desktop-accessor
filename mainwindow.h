#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QMainWindow>
//#include <QGuiApplicationHelper>//用来适配深色模式 Used to adapt to dark model
#include "widget.h"
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QCheckBox>
#include <QFrame>
#include <QRadioButton>
#include <QButtonGroup>
//#include <QPasswordEdit>
#include <QSlider>
#include <unistd.h>

//DWIDGET_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();
private:
    QHBoxLayout *moreSettingLayout = new QHBoxLayout;
    QVBoxLayout *AllWidget = new QVBoxLayout;
    DPushButton *install = new DPushButton;
    DLineEdit *ip = new DLineEdit;
    QButtonGroup *showScreen = new QButtonGroup(this);
    QButtonGroup *rdpVersion = new QButtonGroup(this);
    QButtonGroup *rdesktopVersion = new QButtonGroup(this);
    //QButtonGroup *rdpConnect = new QButtonGroup(this);
    QCheckBox *paste = new DCheckBox();
    QCheckBox *remoteSound = new DCheckBox();
    QCheckBox *shareRootFile = new DCheckBox();
    QCheckBox *shareHomeFile = new DCheckBox();
    QCheckBox *moreSetting = new DCheckBox;
    QFrame *moreSettingFrame = new DFrame;
    QLineEdit *sizeScreenWidth = new DLineEdit();
    QLineEdit *sizeScreenHeight = new DLineEdit();
    QLineEdit *showTitle = new DLineEdit();
    QLineEdit *user = new DLineEdit();
    QLineEdit *command = new DLineEdit();
    //QPasswordEdit *password = new DPasswordEdit();
    QSlider *color = new DSlider();
    QLabel *colorShow = new DLabel();
    QWidget *w=new QWidget; //w是窗口的用户区，应当是所有窗口中控件的父（不包含标题栏及其上边的控件） W is the user area of the window and should be the parent of all controls in the window (excluding the title bar and controls above it)
    //void setTheme(DGuiApplicationHelper::ColorType);
    void ConnectIp();
    void ShowMoreSetting();
    void ShowScreenEnabled();
    char * ReadFile(char path[]);
    char programPath[100];
};

#endif // WIDGET_H
