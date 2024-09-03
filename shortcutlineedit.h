#ifndef SHORTCUTLINEEDIT_H
#define SHORTCUTLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QKeySequence>
#include <windows.h> // For global keyboard hook

class ShortcutLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ShortcutLineEdit(QWidget *parent = nullptr);
    explicit ShortcutLineEdit(const QKeySequence &keySequence, QWidget *parent = nullptr);
    virtual ~ShortcutLineEdit();

    void setKeySequence(const QKeySequence &keySequence);
    QKeySequence keySequence() const;

private:
    void initUI();
    void updateText();
    void handlePrintScreen();

signals:
    void keySequenceChanged(const QKeySequence &keySequence);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QKeySequence m_keySequence;
    static HHOOK hook;
    static ShortcutLineEdit* focusedInstance; // Static pointer to the focused instance
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
};

#endif // SHORTCUTLINEEDIT_H
