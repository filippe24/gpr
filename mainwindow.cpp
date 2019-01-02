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


//MY FUNCTIONS
void MainWindow::setDefaultMesh()
{
    ui->openGLWidget->accesMesh().setDefault();
    ui->openGLWidget->makeCurrent();
    ui->openGLWidget->accesMesh().buildMesh();
    ui->openGLWidget->doneCurrent();
    ui->openGLWidget->update();
}





//MY SLOTS

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



void MainWindow::on_lambdaSlider_actionTriggered(int action)
{
    ui->openGLWidget->accesMesh().setLambda(float(action/100.0f));
}


void MainWindow::on_defaultButton_clicked()
{
    setDefaultMesh();
}

void MainWindow::on_defaultButton_2_clicked()
{
    setDefaultMesh();
}
