#include "mainwindow.h"
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <DWidgetUtil> //加入此头文件方可使用moveToCenter
#include <DSearchEdit>
#include <DTitlebar>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <DLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <DPushButton>
#include <DFileDialog>
#include <iostream>
#include <QByteArray>
#include <DCheckBox>
#include <DMessageBox>
#include <DFrame>
#include <DTabWidget>
#include <DRadioButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QSlider>
#include <DGroupBox>
#include <DTextBrowser>
#include <fstream>
#include <QProcess>
#include <DAbstractDialog>
#include <QLoggingCategory>
#include <QApplication>
// 文件读取需要库
#include <QFile>
// 读取 JSON 需要库
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


#define MAXPATH 1000

/*
 * 打开配置文件
 */
void MainWindow::OpenFile(){
    QString file = DFileDialog::getOpenFileName(this, "浏览", "~", "JSON配置文件(*.json);;所有文件(*.*)");
    if(file != ""){
        return;
    }
    // 读取 JSON 文件
    QFile jsonFile(file);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray array = jsonFile.readAll();
    qDebug() << array;
    QJsonDocument setting = QJsonDocument::fromJson(array);
    QJsonObject obj = setting.object();
    ip->setText(obj.value("ip").toString());
    moreSetting->setChecked(obj.value("moreSetting").toBool());
    user->setText(obj.value("user").toString());
    password->setText(obj.value("password").toString());
    showScreen->setId(rdesktopVersion->buttons()[obj.value("showScreen").toInt()], -1);
    sizeScreenWidth->setText(obj.value("sizeScreenWidth").toString());
    sizeScreenHeight->setText(obj.value("sizeScreenHeight").toString());
    showTitle->setText(obj.value("showTitle").toString());
    color->setValue(obj.value("color").toInt());
    rdpVersion->setId(rdpVersion->buttons()[obj.value("rdpVersion").toInt()], -1);
    rdesktopVersion->setId(rdesktopVersion->buttons()[obj.value("rdpVersion").toInt()], -1);
    paste->setChecked(obj.value("paste").toBool());
    remoteSound->setChecked(obj.value("remoteSound").toBool());
    shareRootFile->setChecked(obj.value("shareRootFile").toBool());
    shareHomeFile->setChecked(obj.value("shareHomeFile").toBool());
    command->setText(obj.value("command").toString());
}
/*
 * 保存配置文件
*/
void MainWindow::SaveFile(){
    QString file = DFileDialog::getSaveFileName(this, "浏览", "~", "JSON配置文件(*.json);;所有文件(*.*)");
    if(file != ""){
        return;
    }
    QJsonObject json;
    json.insert("ip", ip->text());
    json.insert("moreSetting", moreSetting->isChecked());
    json.insert("user", user->text());
    json.insert("password", password->text());
    json.insert("sizeScreenWidth", sizeScreenWidth->text());
    json.insert("sizeScreenHeight", sizeScreenHeight->text());
    json.insert("showTitle", showTitle->text());
    json.insert("color", color->value());
    json.insert("rdpVersion", rdpVersion->checkedId());
    json.insert("rdesktopVersion", rdesktopVersion->checkedId());
    json.insert("paste", paste->isChecked());
    json.insert("remoteSound", remoteSound->isChecked());
    json.insert("shareRootFile", shareRootFile->isChecked());
    json.insert("shareHomeFile", shareHomeFile->isChecked());
    json.insert("showScreen", showScreen->checkedId());
    json.insert("command", command->text());
    QJsonDocument jsonDocument;
    jsonDocument.setObject(json);
    QFile jsonFile(file);
    jsonFile.open(QIODevice::ReadWrite);
    jsonFile.write(jsonDocument.toJson());
    jsonFile.close();
}
/*
 * Use Deepin Terminal Open Program
*/
void MainWindow::OpenProgramWithTerminal(QStringList argv){
    QStringList command;
    command << "deepin-terminal" << "--keep-open" <<"-e" << argv;
    QProcess process;
    process.start(QApplication::applicationDirPath() + "/launch.sh", command);
    process.waitForFinished();
    process.close();
}
/*
 * Install xrdp
*/
void MainWindow::InstallXrdp(){
    QStringList install;
    install << "pkexec" << "apt" << "install" << "xrdp" << "-y";
    OpenProgramWithTerminal(install);
}
void MainWindow::WriteXrdpUserXfce4(){
    QFile file(QDir::homePath() + "/.xsession");
    file.open(QIODevice::WriteOnly);
    if(file.write("xfce4-session") == -1){
        QMessageBox::critical(this, "错误", "写入错误！");
    }
    else{
        QMessageBox::information(this, "提示", "写入成功");
    }
    file.close();

}
void MainWindow::WriteXrdpUserDde(){
    QFile file(QDir::homePath() + "/.xsession");
    file.open(QIODevice::WriteOnly);
    if(file.write("startdde-x11") == -1){
        QMessageBox::critical(this, "错误", "写入错误！");
    }
    else{
        QMessageBox::information(this, "提示", "写入成功");
    }
    file.close();
}

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)
{
    // 允许 qDebug() 输出
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    // 获取程序目录
    getcwd(programPath, 100);
    std::clog << "Path: " << programPath << std::endl;
    //初始化主窗口 Initialize the main window
    setCentralWidget(w);//将w作为窗口的用户部分（整个窗口除了标题栏的部分）
    moveToCenter(this); //把窗口移动到屏幕中间
    w->setWindowTitle(tr("远程桌面连接器"));
    this->setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    MainWindow::resize(500, 100); //改变窗口大小应当改变MainWindow的大小

    ip = new DLineEdit;
    ip->setParent(w);
    ip->setToolTip(tr("请在这里输入IP地址"));
    ip->setPlaceholderText(tr("请在这里输入IP地址"));

    ip->show();

    DPushButton *connectButton = new DPushButton;
    connectButton->setText(tr("连接"));
    connectButton->setParent(w);
    connect(connectButton, &DPushButton::clicked, this, &MainWindow::ConnectIp);
    connectButton->show();

    moreSetting = new DCheckBox("更多选项");
    moreSetting->setParent(w);
    moreSetting->setCheckable(true);
    moreSetting->show();
    connect(moreSetting, &DCheckBox::stateChanged, this, &MainWindow::ShowMoreSetting);

    QHBoxLayout *moreSettingLayout = new QHBoxLayout;
    moreSettingLayout->setParent(moreSettingFrame);

    moreSettingFrame = new DFrame;
    moreSettingFrame->setParent(w);
    moreSettingFrame->setEnabled(false);
    moreSettingFrame->setLayout(moreSettingLayout);

    DWidget *showTab = new DWidget;
    showTab->setParent(moreSettingFrame);
    showTab->show();
    DWidget *userPasswordTab = new DWidget();
    userPasswordTab->setParent(moreSettingFrame);
    userPasswordTab->show();

    user = new DLineEdit();
    user->setParent(userPasswordTab);
    user->setPlaceholderText("在这里输入用户名（不输入则以当前电脑登录用户的用户名）");
    user->show();

    password = new DPasswordEdit();
    password->show();

    showScreen = new QButtonGroup();
    DRadioButton *fullScreen = new DRadioButton("全屏访问（按Ctrl+Alt+Enter退出全屏）");
    fullScreen->show();
    fullScreen->setParent(showTab);
    DRadioButton *sizeScreen = new DRadioButton("指定分辨率访问");
    sizeScreen->setChecked(true);
    sizeScreen->setParent(showTab);
    sizeScreen->show();

    showScreen->addButton(fullScreen, 0);
    showScreen->addButton(sizeScreen, 1);
    connect(moreSetting, &DCheckBox::stateChanged, this, &MainWindow::ShowMoreSetting);
    connect(fullScreen, &DRadioButton::clicked, this, &MainWindow::ShowScreenEnabled);
    connect(sizeScreen, &DRadioButton::clicked, this, &MainWindow::ShowScreenEnabled);


    sizeScreenWidth = new DLineEdit();
    sizeScreenWidth->setParent(showTab);
    sizeScreenWidth->setText("800");
    sizeScreenWidth->show();
    sizeScreenHeight = new DLineEdit();
    DLabel *cheng = new DLabel(tr("×"));
    cheng->setParent(showTab);
    cheng->show();
    sizeScreenHeight->setParent(showTab);
    sizeScreenHeight->setText("600");
    sizeScreenWidth->show();

    DLabel *showTitleTips = new DLabel("标题：");
    showTitleTips->setParent(showTab);
    showTitleTips->show();

    showTitle = new DLineEdit();
    showTitle->setParent(showTab);
    showTitle->show();

    DLabel *colorTips = new DLabel(tr("色彩位数："));
    colorTips->setParent(showTab);
    colorTips->show();

    QHBoxLayout *colorLayout = new QHBoxLayout();

    color = new DSlider();
    color->setMaximum(3);
    color->setMinimum(0);
    color->setValue(3);
    color->setParent(showTab);
    color->show();
    connect(color, &DSlider::valueChanged, this, [&](){
        QString number = QString::number(MainWindow::color->value() * 8 + 8);
        number.append("位");
        colorShow->setText(number);
    });

    colorShow = new DLabel("32位");
    colorShow->setParent(showTab);
    colorShow->show();

    colorLayout->addWidget(color);
    colorLayout->addWidget(colorShow);

    QGridLayout *showTabLayout = new QGridLayout;
    showTabLayout->setParent(showTab);
    showTabLayout->addWidget(fullScreen, 0, 0, 1, 4);
    showTabLayout->addWidget(sizeScreen, 1, 0);
    showTabLayout->addWidget(sizeScreenWidth, 1, 1);
    showTabLayout->addWidget(cheng, 1, 2);
    showTabLayout->addWidget(sizeScreenHeight, 1, 3);
    showTabLayout->addWidget(showTitleTips, 2, 0);
    showTabLayout->addWidget(showTitle, 2, 1, 1, 3);
    showTabLayout->addWidget(colorTips, 3, 0);
    showTabLayout->addLayout(colorLayout, 3, 1, 1, 3);
    showTab->setLayout(showTabLayout);

    QGridLayout *userPasswordLayout = new QGridLayout();
    userPasswordLayout->addWidget(new DLabel(tr("用户名：")), 0, 0);
    userPasswordLayout->addWidget(user, 0, 1);
    userPasswordLayout->addWidget(new DLabel(tr("密码：")), 1, 0);
    userPasswordLayout->addWidget(password, 1, 1);
    userPasswordTab->setLayout(userPasswordLayout);
    QVBoxLayout *connectLayout = new QVBoxLayout();
    QVBoxLayout *rdpVersionLayout = new QVBoxLayout();
    QVBoxLayout *rdesktopVersionLayout = new QVBoxLayout();
    QVBoxLayout *rdesktopConnectLayout = new QVBoxLayout();

    DWidget *connectTab = new DWidget();
    connectTab->setParent(moreSettingFrame);
    connectTab->setLayout(connectLayout);
    connectTab->show();

    DGroupBox *rdpVersionBox = new DGroupBox();
    rdpVersionBox->setParent(connectTab);
    rdpVersionBox->show();
    rdpVersionBox->setTitle("rdp 版本");
    rdpVersionBox->setLayout(rdpVersionLayout);
    connectLayout->addWidget(rdpVersionBox);

    DRadioButton *rdp4 = new DRadioButton();
    rdp4->setParent(rdpVersionBox);
    rdp4->setText("设置为rdp4协议连接");
    rdp4->show();

    DRadioButton *rdp5 = new DRadioButton();
    rdp5->setParent(rdpVersionBox);
    rdp5->setText("设置为rdp5协议连接（推荐）");
    rdp5->setChecked(true);
    rdp5->show();

    rdpVersion = new QButtonGroup();
    rdpVersion->addButton(rdp4, 0);
    rdpVersion->addButton(rdp5, 1);

    rdpVersionLayout->addWidget(rdp4);
    rdpVersionLayout->addWidget(rdp5);

    DGroupBox *rdesktopVersionBox = new DGroupBox();
    rdesktopVersionBox->setParent(connectTab);
    rdesktopVersionBox->show();
    rdesktopVersionBox->setTitle("rdesktop 版本");
    rdesktopVersionBox->setLayout(rdesktopVersionLayout);
    connectLayout->addWidget(rdesktopVersionBox);

    DRadioButton *systemRdesktop = new DRadioButton();
    systemRdesktop->setParent(rdesktopVersionBox);
    systemRdesktop->setText("设置为系统版本的rdesktop（可能会不支持连接Windows XP）");
    systemRdesktop->setChecked(true);
    systemRdesktop->show();

    DRadioButton *rdesktop190 = new DRadioButton();
    rdesktop190->setParent(rdesktopVersionBox);
    rdesktop190->setText("设置为rdesktop 1.9.0（测试支持连接Windows XP）");
    rdesktop190->show();

    rdesktopVersion = new QButtonGroup();
    rdesktopVersion->addButton(systemRdesktop, 0);
    rdesktopVersion->addButton(rdesktop190, 1);

    rdesktopVersionLayout->addWidget(systemRdesktop);
    rdesktopVersionLayout->addWidget(rdesktop190);
    QString rdesktop = "rdesktop";
    // Read System To Sure Install Rdesktop
    // Else use build rdesktop 1.9.0
    if(system("which rdesktop")){
        rdesktop190->setChecked(true);
        rdesktop = QApplication::applicationDirPath() + "/rdesktop";
    }

    DGroupBox *rdesktopConnect = new DGroupBox();
    rdesktopConnect->setParent(connectTab);
    rdesktopConnect->show();
    rdesktopConnect->setTitle("连接选项");
    rdesktopConnect->setLayout(rdesktopConnectLayout);
    connectLayout->addWidget(rdesktopConnect);

    paste = new DCheckBox();
    paste->setParent(rdesktopConnect);
    paste->setText("剪切板共享");
    paste->setChecked(true);
    paste->show();

    remoteSound = new DCheckBox();
    remoteSound->setParent(rdesktopConnect);
    remoteSound->setText("播放远程连接声音");
    remoteSound->setChecked(true);
    remoteSound->show();

    shareRootFile = new DCheckBox();
    shareRootFile->setParent(rdesktopConnect);
    shareRootFile->setText("共享系统根目录");
    shareRootFile->show();

    shareHomeFile = new DCheckBox();
    shareHomeFile->setParent(rdesktopConnect);
    shareHomeFile->setText("共享用户目录");
    shareHomeFile->setChecked(true);
    shareHomeFile->show();

    rdesktopConnectLayout->addWidget(paste);
    rdesktopConnectLayout->addWidget(remoteSound);
    rdesktopConnectLayout->addWidget(shareRootFile);
    rdesktopConnectLayout->addWidget(shareHomeFile);

    DWidget *highSetting = new DWidget();
    highSetting->setParent(moreSettingFrame);
    highSetting->show();

    QVBoxLayout *highSettingLayout = new QVBoxLayout();
    highSettingLayout->setParent(highSetting);
    highSetting->setLayout(highSettingLayout);

    DTextBrowser *help = new DTextBrowser();
    help->setParent(highSetting);
    QProcess rdesktopHelp;
    rdesktopHelp.start(rdesktop);
    rdesktopHelp.waitForFinished();
    help->setText(rdesktopHelp.readAllStandardError());
    help->show();
    highSettingLayout->addWidget(help);

    command = new DLineEdit();
    command->setPlaceholderText("请在这里输入需要的参数（上面是参数示例）");
    command->setParent(highSetting);
    command->show();
    highSettingLayout->addWidget(command);

    DTabWidget *moreSettingTab = new DTabWidget;
    moreSettingTab->setParent(moreSettingFrame);
    moreSettingTab->addTab(showTab, tr("显示"));
    moreSettingTab->addTab(userPasswordTab, tr("用户"));
    moreSettingTab->addTab(connectTab, tr("连接"));
    moreSettingTab->addTab(highSetting, tr("高级"));
    moreSettingTab->show();

    moreSettingLayout->addWidget(moreSettingTab);

    QVBoxLayout *AllWidget = new QVBoxLayout;
    QHBoxLayout *firstLineWidget = new QHBoxLayout;
    firstLineWidget->addWidget(new DLabel("IP地址："));
    firstLineWidget->addWidget(ip);
    firstLineWidget->addWidget(connectButton);

    AllWidget->addLayout(firstLineWidget);
    AllWidget->setStretchFactor(firstLineWidget, 1);
    AllWidget->addWidget(moreSetting);
    AllWidget->setStretchFactor(moreSetting, 1);
    AllWidget->addWidget(moreSettingFrame);
    AllWidget->setStretchFactor(moreSettingFrame, 6);

    w->setLayout(AllWidget);

    //在标题栏上添加一个菜单/菜单项 Add a menu/menu item to the title bar
    QMenu *menu=new QMenu;
    QAction *openFile = new QAction("打开配置文件");
    connect(openFile, &QAction::triggered, this, &MainWindow::OpenFile);
    menu->addAction(openFile);

    QAction *saveFile = new QAction("保存配置文件");
    connect(saveFile, &QAction::triggered, this, &MainWindow::SaveFile);
    menu->addAction(saveFile);

    menu->addSeparator();

    QMenu *xrdp = menu->addMenu("设置RDP服务器(基于 xrdp)");
    QAction *installXrdp = new QAction("安装 xrdp");
    xrdp->addAction(installXrdp);
    connect(installXrdp, &QAction::triggered, this, &MainWindow::InstallXrdp);

    xrdp->addSeparator();

    QAction *writeXrdpUserXfce4 = new QAction("设置当前用户远程连接使用 xfce4");
    connect(writeXrdpUserXfce4, &QAction::triggered, this, &MainWindow::WriteXrdpUserXfce4);
    xrdp->addAction(writeXrdpUserXfce4);

    QAction *writeXrdpUserDde = new QAction("设置当前用户远程连接使用 dde");
    connect(writeXrdpUserDde, &QAction::triggered, this, &MainWindow::WriteXrdpUserDde);
    xrdp->addAction(writeXrdpUserDde);



    menu->addSeparator();

    QAction *giteeAction=new QAction("Gitee 网站");
    menu->addAction(giteeAction);
    connect(giteeAction, &QAction::triggered, this, [&](){std::system("xdg-open https://gitee.com/gfdgd-xi/simple-remote-desktop-accessor");});


    QAction *githubAction=new QAction("Github 网站");
    menu->addAction(githubAction);
    connect(githubAction, &QAction::triggered, this, [&](){std::system("xdg-open https://github.com/gfdgd-xi/simple-remote-desktop-accessor");});

    QAction *gitlinkAction = new QAction("Gitlink 网站");
    menu->addAction(gitlinkAction);
    connect(gitlinkAction, &QAction::triggered, this, [&](){std::system("");});

    titlebar()->setMenu(menu);

    //设置标题栏上的图标 Set the icon on the title bar
    titlebar()->setIcon(QIcon::fromTheme("/opt/durapps/spark-simple-remote-desktop-accessor/icon.svg"));

    //让程序适配浅色模式和深色模式 Adapt the program to light and dark model
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,this,&MainWindow::setTheme);

}


void MainWindow::ShowScreenEnabled(){
    MainWindow::sizeScreenWidth->setEnabled(MainWindow::showScreen->checkedId());
    MainWindow::sizeScreenHeight->setEnabled(MainWindow::showScreen->checkedId());
}
void MainWindow::ShowMoreSetting(){
    if (MainWindow::moreSetting->checkState()){
        moreSettingFrame->setEnabled(true);
        return;
    }
    moreSettingFrame->setEnabled(false);
}
void MainWindow::ConnectIp(){
    if(MainWindow::ip->text() == ""){
        DMessageBox::information(w, tr("提示"), tr("没有输入IP地址，无法继续"));
        return;
    }
    QStringList option;
    QString App = "padsp";
    if (moreSetting->isChecked()){
        switch (MainWindow::rdesktopVersion->checkedId()) {
            case 0:
                option << "rdesktop";
                break;
            case 1:
                option << QApplication::applicationDirPath() + "/rdesktop";
                break;
        }
        option << MainWindow::ip->text() << "-T" << MainWindow::showTitle->text() << "-a" << QString("%1").arg(MainWindow::color->value() * 8 + 8);
        switch (MainWindow::showScreen->checkedId()){
            case 0:
                option << "-f";
                break;
            case 1:
                option << "-g" << MainWindow::sizeScreenWidth->text() + "x" + MainWindow::sizeScreenHeight->text();
                break;
        }
        if(MainWindow::user->text() != ""){
            option << "-u" << MainWindow::user->text();
        }
        if(MainWindow::password->text()!=""){
            option << "-p" << MainWindow::password->text();
        }
        switch (MainWindow::rdpVersion->checkedId()) {
            case 0:
                option << "-4";
                break;
            case 1:
                option << "-5";
                break;
        }
        if(MainWindow::paste->isChecked()){
            option << "-r" << "clipboard:PRIMARYCLIPBOARD";
        }
        if(MainWindow::remoteSound->isChecked()){
            option << "-r" << "sound:local";
        }
        if(MainWindow::shareRootFile->isChecked()){
            option << "-r" << "disk:ROOT=/";
        }
        if(MainWindow::shareHomeFile->isChecked()){
            option  << "-r" << "disk:HOME=" + QDir::homePath();
        }
        if(MainWindow::command->text() != ""){
            option << MainWindow::command->text().split(" ");
        }
    }
    else {
        QString rdesktop = "rdesktop";
        // Read System To Sure Install Rdesktop
        // Else use build rdesktop 1.9.0
        if(system("which rdesktop")){
            rdesktop = QApplication::applicationDirPath() + "/rdesktop";
        }
        option << rdesktop << MainWindow::ip->text() << "-T" << MainWindow::ip->text() << "-r" << "clipboard:PRIMARYCLIPBOARD" << "-r" << "sound:local" << "-r" << "disk:HOME=" + QDir::homePath();
    }
    w->setEnabled(false);
    QProcess *process = new QProcess();
    process->start(App, option);
    process->write("yes\n");
    process->waitForStarted();
    process->waitForFinished();
    process->close();
    if(process->exitCode() != 0 && process->exitCode() != 62){
        DMessageBox::critical(this, "错误", "程序返回值：" + QString("%1").arg(process->exitCode()) + "\n具体错误：\n" + process->readAllStandardError());
        qDebug() << QString::fromLocal8Bit(process->readAllStandardError());
    }
    w->setEnabled(true);
}
MainWindow::~MainWindow()
{
    delete w;
}

void MainWindow::setTheme(DGuiApplicationHelper::ColorType theme)
{
    //这个函数在程序运行之初就会被调用一次，而后每次更换主题颜色时被调用。 This function is called at the beginning of the program and then every time the theme color is changed.
    if(theme==DGuiApplicationHelper::LightType){ //在这个位置设置两种不同的样式 Set two different styles at this location
        //浅色模式 Light Mode
        qDebug()<<"Lighe";
    }else {
        //深色模式 Dark Mode
        qDebug()<<"Dark";
    }
}
