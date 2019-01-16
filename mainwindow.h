#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_action_Quit_triggered();

	void on_checkBoxFill_toggled(bool checked);

	void on_action_Open_triggered();

    void on_Mean_clicked(bool checked);

    void on_Gaussian_clicked(bool checked);

    void on_checkBoxRefl_clicked(bool checked);


    void on_ComputeCurvature_clicked();

    void on_LaplacianPush_clicked();

    void on_lambdaSlider_actionTriggered(int action);

//my functions
    void on_defaultButton_clicked();

    void on_defaultButton_2_clicked();

    void on_LaplacianCotgPush_clicked();

    void on_globalSmoothPush_clicked();


    void on_iterativeSelection_activated(int index);

    void on_gaussianWeight_sliderMoved(int position);

    void on_meanWeight_sliderMoved(int position);

    void on_parametrizeButton_clicked();

    void on_globalSmoothCotangPush_clicked();

    void on_percentageSpinBox_valueChanged(int arg1);

    void on_magnifyUniform_clicked();

    void on_magnifyCotangent_clicked();

    void on_iteration1Spin_valueChanged(int arg1);

    void on_iteration2Spin_valueChanged(int arg1);

    void on_lambdaMagSpin_valueChanged(double arg1);

private:
    void setDefaultMesh();


private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
