#ifndef XKEYSEQUENCEEDIT_H
#define XKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>
#include <windows.h> // For LOWLEVELKEYBOARDPROC

class XKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    explicit XKeySequenceEdit(QWidget *parent = nullptr);
    explicit XKeySequenceEdit(const QKeySequence &keySequence, QWidget *parent = nullptr);
    virtual ~XKeySequenceEdit();

private:
    void initUI();
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void handlePrintScreen();

    static HHOOK hook;
    static XKeySequenceEdit* instance; // Static instance pointer

signals:
    void sigKeySeqChanged(const QKeySequence &keySequence);
    void printScreenDetected(); // Add a signal for Print Screen detection

public slots:
    void handlePrintScreenSlot(); // Slot function

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // XKEYSEQUENCEEDIT_H
