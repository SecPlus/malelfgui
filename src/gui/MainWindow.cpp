#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    util = new Util();

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
    delete currentFile;
    delete util;
}

void MainWindow::init(){

    this->viewASCII = true;
    this->colorELF = true;
    ui->actionASCII->setChecked(true);
    ui->actionColor_ELF->setChecked(true);

    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);
}

void MainWindow::on_actionQuit_triggered()
{
    controller->Quit();
}

void MainWindow::on_actionOpen_ELF_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));

    QFile* file = new QFile(fileName);
        if (!file->open(QFile::ReadWrite)) {
            QMessageBox::warning(this, tr("SDI"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file->errorString()));
            return;
        }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->hexDump->setData(file->readAll());
    QApplication::restoreOverrideCursor();

    this->setCurrentFile(file);

    QString cmd = "malelf dissect -i " + fileName;
    QString result = util->executeMalelf(cmd);
    ui->log->setText(result);

    //Colorize ELF sections
    //ui->hexDump->


}

void MainWindow::on_actionClose_ELF_triggered()
{
    if(this->currentFile)
        this->currentFile->close();

    ui->hexDump->setData(NULL);
}

void MainWindow::setCurrentFile(QFile* file)
{
    this->currentFile = file;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About"));
    msgBox.setInformativeText(tr("MalelficusGUI is a Graphical User Interface for Malelficus"));
    msgBox.exec();
}

void MainWindow::on_actionSave_ELF_triggered()
{
    if(this->currentFile)
        this->currentFile->write(ui->hexDump->data());
}

void MainWindow::on_actionSave_ELF_As_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));

    QFile* file = new QFile(fileName);
        if (!file->open(QFile::ReadWrite)) {
            QMessageBox::warning(this, tr("SDI"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file->errorString()));
            return;
        }

    file->write(ui->hexDump->data());

    this->setCurrentFile(file);

}

void MainWindow::on_actionDisassemble_triggered()
{
    if(this->currentFile){
        QString cmd = "malelf disas -i " + this->currentFile->fileName();
        QString result = util->executeMalelf(cmd);
        ui->log->setText(result);
    }
}

void MainWindow::on_actionReverse_into_C_triggered()
{
    if(this->currentFile){
        QString cmd = "malelf reverse_elf -r -i " + this->currentFile->fileName();
        QString result = util->executeMalelf(cmd);
        ui->log->setText(result);
    }
}

void MainWindow::on_actionColor_ELF_triggered()
{
    this->colorELF = ui->actionColor_ELF->isChecked();
    ui->hexDump->setColorELF(this->colorELF);
}

void MainWindow::on_actionASCII_triggered()
{
    this->viewASCII = ui->actionASCII->isChecked();
    ui->hexDump->setAsciiArea(this->viewASCII);
}