#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QWidget>

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

#endif // TERMINALWINDOW_H
