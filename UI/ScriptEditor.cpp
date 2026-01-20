#include "ScriptEditor.h"

#include <qdatetime.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

ScriptEditor::ScriptEditor(const std::string& filePath, QWidget* parent)
    : QDialog(parent), m_filePath(filePath)
{
    setWindowTitle("Script Editor - " + QString::fromStdString(filePath));
    resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    m_editor = new QPlainTextEdit(this);
    m_editor->setStyleSheet("background-color: #1e1e1e; color: #dcdcdc; font-family: Consolas, Monospace; font-size: 14px;");
    layout->addWidget(m_editor);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    
    m_statusLabel = new QLabel(this);
    btnLayout->addWidget(m_statusLabel);
    btnLayout->addStretch();

    QPushButton* saveBtn = new QPushButton("Save", this);
    connect(saveBtn, &QPushButton::clicked, this, &ScriptEditor::onSave);
    btnLayout->addWidget(saveBtn);

    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(closeBtn);

    layout->addLayout(btnLayout);

    loadFile();
}

void ScriptEditor::loadFile() {
    QFile file(QString::fromStdString(m_filePath));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_editor->setPlainText(in.readAll());
    } else {
        m_editor->setPlainText(""); 
    }
}

void ScriptEditor::onSave() {
    QFile file(QString::fromStdString(m_filePath));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_editor->toPlainText();
        m_statusLabel->setText("Saved at " + QTime::currentTime().toString());
    } else {
        QMessageBox::critical(this, "Error", "Could not save file!");
    }
}