#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QProcess>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

class TerminalWindow : public QMainWindow {
  Q_OBJECT

public:
  TerminalWindow(QWidget *parent = nullptr);

private slots:
  void onCommandEntered();

private:
  QVBoxLayout *layout;
  QPlainTextEdit *outputText;
  QLineEdit *inputLine;
  QProcess *process;
};

TerminalWindow::TerminalWindow(QWidget *parent) : QMainWindow(parent) {
  // Create main layout
  layout = new QVBoxLayout;
  setLayout(layout);

  // Output text box
  outputText = new QPlainTextEdit();
  outputText->setReadOnly(true);
  layout->addWidget(outputText);

  // Input line
  inputLine = new QLineEdit;
  layout->addWidget(inputLine);

  // Run button
  QPushButton *runButton = new QPushButton("Run");
  connect(runButton, &QPushButton::clicked, this, &TerminalWindow::onCommandEntered);
  layout->addWidget(runButton);

  // Process object
  process = new QProcess;
}

void TerminalWindow::onCommandEntered() {
  QString command = inputLine->text();
  if (command.isEmpty()) {
    return;
  }

  // Clear previous output
  outputText->clear();

  // Split command and arguments
  QStringList arguments = command.split(" ", QString::SkipEmptyParts);

  // Set program and arguments
  process->setProgram(arguments[0]);
  if (arguments.size() > 1) {
    process->setArguments(arguments.mid(1));
  }

  // Connect signals for capturing output
  connect(process, &QProcess::readyReadStandardOutput(), this, [&]() {
    outputText->appendPlainText(process->readAllStandardOutput());
  });

  connect(process, &QProcess::readyReadStandardError(), this, [&]() {
    outputText->appendPlainText(process->readAllStandardError());
  });

  // Start process
  process->start();

  // Clear input line after process starts
  inputLine->clear();
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  TerminalWindow window;
  window.show();
  return app.exec();
}

#include "terminalwindow.h"
