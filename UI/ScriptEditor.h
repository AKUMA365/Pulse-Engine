#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QDialog>
#include <string>

class QPlainTextEdit;
class QPushButton;
class QLabel;

class ScriptEditor : public QDialog {
    Q_OBJECT

public:
    explicit ScriptEditor(const std::string& filePath, QWidget* parent = nullptr);

private slots:
    void onSave();

private:
    void loadFile();
    
    std::string m_filePath;
    QPlainTextEdit* m_editor;
    QLabel* m_statusLabel;
};

#endif