#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(1);
    QStringList list(tr("Node"));
    list << tr("Type");
    ui->treeWidget->setHeaderLabels(list);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFileOpen()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Hi");
    int result = shapefile.Open(filepath.toStdString());
    if (result != 0) {
        QMessageBox::warning(this, tr("Bad"), tr("Something is wrong"));
    }
    else {
        int count = shapefile.GetEntityCount();
        ui->shapeEntityCount->setText(QString::number(count));
        QString typeString = QString::fromStdString(shapefile.GetTypeString());
        ui->shapeType->setText(typeString);
        QList<QTreeWidgetItem *> items;
        for (int i = 0; i < count; i++) {
            ShapeObject object;
            if (shapefile.GetShape(i, object) >= 0) {
                QStringList list(QString("%1").arg(i));
                list << QString::fromStdString(object.GetTypeString());
                items.append(
                    new QTreeWidgetItem(
                        (QTreeWidget*)0,
                        list
                    )
                );
            }
        }
        ui->treeWidget->clear();
        ui->treeWidget->insertTopLevelItems(0, items);
    }
}
