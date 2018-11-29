#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
	 QApplication::quit();
}

void MainWindow::on_checkBoxFill_toggled(bool checked)
{
	 ui->openGLWidget->setPolygonMode(checked);
}

void MainWindow::on_action_Open_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open PLY"), ".", tr("*.ply"));

	ui->openGLWidget->loadMesh(filename);
}


void MainWindow::on_Mean_clicked(bool checked)
{
    ui->openGLWidget->select_gauss_mean(checked);

}

void MainWindow::on_Gaussian_clicked(bool checked)
{
    ui->openGLWidget->select_gauss_mean(not(checked));
}

void MainWindow::on_checkBoxRefl_clicked(bool checked)
{
    ui->openGLWidget->set_reflection_lines(checked);
}


void MainWindow::on_ComputeCurvature_clicked()
{
    ui->openGLWidget->compute_curvatures();
}

void MainWindow::on_LaplacianPush_clicked()
{
    ui->openGLWidget->laplacian_operator();
}
