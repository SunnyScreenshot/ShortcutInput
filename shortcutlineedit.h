#ifndef SHORTCUTLINEEDIT_H
#define SHORTCUTLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QKeySequence>

#ifdef Q_OS_WIN
#include <windows.h> // For global keyboard hook
#endif

class ShortcutLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ShortcutLineEdit(QWidget *parent = nullptr);
    explicit ShortcutLineEdit(const QKeySequence &keySequence, QWidget *parent = nullptr);
    virtual ~ShortcutLineEdit();

    void setKeySequence(const QKeySequence &keySequence);
    QKeySequence keySequence() const;

signals:
    void keySequenceChanged(const QKeySequence &keySequence);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void initUI();
    void updateText();
    void handlePrintScreen();

#ifdef Q_OS_WIN
    static HHOOK hook;
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
#endif

    QKeySequence m_keySequence;
    static ShortcutLineEdit* focusedInstance; // Static pointer to the focused instance
};

#endif // SHORTCUTLINEEDIT_H
