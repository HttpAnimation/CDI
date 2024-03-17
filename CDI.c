#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Get the command to execute (if any) from program arguments
  QString command;
  if (argc > 1) {
    command = argv[1];
  } else {
    qWarning("No command provided. Please specify a command to execute.");
    return 1;
  }

  // Create a QProcess object
  QProcess process;

  // Split the command and arguments (if any)
  QStringList arguments = command.split(" ", QString::SkipEmptyParts);

  // Set the program to execute
  process.setProgram(arguments[0]);

  // Set any additional arguments (if present)
  if (arguments.size() > 1) {
    process.setArguments(arguments.mid(1));
  }

  // Connect signals for monitoring process execution (optional)
  QObject::connect(&process, SIGNAL(readyReadStandardOutput()), &process, SLOT(readAllStandardOutput()));
  QObject::connect(&process, SIGNAL(readyReadStandardError()), &process, SLOT(readAllStandardError()));

  // Start the process
  process.start();

  // Wait for the process to finish
  process.waitForFinished();

  // Print the output (optional)
  if (process.exitStatus() == QProcess::NormalExit) {
    qDebug() << "Standard Output:\n" << process.readAllStandardOutput();
  } else {
    qWarning() << "Error: Process failed with exit code" << process.exitStatus();
    qDebug() << "Standard Error:\n" << process.readAllStandardError();
  }

  return 0;
}
