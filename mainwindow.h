#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <DMainWindow>
#include <DGuiApplicationHelper>//用来适配深色模式 Used to adapt to dark model
#include "widget.h"
#include <DPushButton>
#include <DLabel>
#include <QString>
#include <DLineEdit>
#include <DCheckBox>
#include <DFrame>
#include <DRadioButton>
#include <QButtonGroup>
#include <DPasswordEdit>

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(DMainWindow *parent = nullptr);
    ~MainWindow();
private:
    QHBoxLayout *moreSettingLayout = new QHBoxLayout;
    QVBoxLayout *AllWidget = new QVBoxLayout;
    DPushButton *install = new DPushButton;
    DLineEdit *ip = new DLineEdit;
    QButtonGroup *showScreen = new QButtonGroup(this);
    DCheckBox *moreSetting = new DCheckBox;
    DFrame *moreSettingFrame = new DFrame;
    DLineEdit *sizeScreenWidth = new DLineEdit();
    DLineEdit *sizeScreenHeight = new DLineEdit();
    DLineEdit *showTitle = new DLineEdit();
    DLineEdit *user = new DLineEdit();
    DPasswordEdit *password = new DPasswordEdit();
    QWidget *w=new QWidget; //w是窗口的用户区，应当是所有窗口中控件的父（不包含标题栏及其上边的控件） W is the user area of the window and should be the parent of all controls in the window (excluding the title bar and controls above it)
    void setTheme(DGuiApplicationHelper::ColorType);
    void ConnectIp();
    void ShowMoreSetting();
    void ShowScreenEnabled();
};

#endif // WIDGET_H
