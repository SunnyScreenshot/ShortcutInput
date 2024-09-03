#include "shortcutlineedit.h"
#include <QFocusEvent>
#include <QDebug>

HHOOK ShortcutLineEdit::hook = nullptr;
ShortcutLineEdit* ShortcutLineEdit::focusedInstance = nullptr;

ShortcutLineEdit::ShortcutLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    initUI();
}

ShortcutLineEdit::ShortcutLineEdit(const QKeySequence &keySequence, QWidget *parent)
    : QLineEdit(parent), m_keySequence(keySequence)
{
    initUI();
}

ShortcutLineEdit::~ShortcutLineEdit()
{
    // Cleanup the hook when no more instances exist
    if (hook && !focusedInstance) {
        UnhookWindowsHookEx(hook);
        hook = nullptr;
    }
}

void ShortcutLineEdit::setKeySequence(const QKeySequence &keySequence)
{
    if (m_keySequence != keySequence) {
        m_keySequence = keySequence;
        updateText();
        emit keySequenceChanged(m_keySequence);
    }
}

QKeySequence ShortcutLineEdit::keySequence() const
{
    return m_keySequence;
}


void ShortcutLineEdit::initUI()
{
    setAlignment(Qt::AlignCenter);  // Center-align text
    setReadOnly(true);              // Make the line edit read-only initially
    setPlaceholderText(tr("Press shortcut"));
    if (!hook) {
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    }
}

void ShortcutLineEdit::updateText()
{
    if (m_keySequence.isEmpty()) {
        setText(QString());
    } else {
        setText(m_keySequence.toString(QKeySequence::NativeText));
    }
}

void ShortcutLineEdit::handlePrintScreen()
{
    QKeySequence keySequence(Qt::Key_Print);
    setKeySequence(keySequence);
    emit keySequenceChanged(keySequence);
}


void ShortcutLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        setKeySequence(QKeySequence()); // Clear key sequence
    } else if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control || event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta) {
        return; // Do not update key sequence for modifier keys only
    } else {
        QKeySequence newSequence(event->key() | event->modifiers());
        setKeySequence(newSequence);
    }

    // Prevent the default behavior of QLineEdit
    event->ignore();
}

void ShortcutLineEdit::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore(); // Ignore to prevent default behavior
}

void ShortcutLineEdit::focusInEvent(QFocusEvent *event)
{
    focusedInstance = this; // Set the static pointer to this instance
    QLineEdit::focusInEvent(event);
}

void ShortcutLineEdit::focusOutEvent(QFocusEvent *event)
{
    if (focusedInstance == this) {
        focusedInstance = nullptr; // Clear the static pointer if this instance loses focus
    }
    QLineEdit::focusOutEvent(event);
}


LRESULT CALLBACK ShortcutLineEdit::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
        if (wParam == WM_KEYDOWN && pKeyboard->vkCode == VK_SNAPSHOT) {
            if (focusedInstance) {
                focusedInstance->handlePrintScreen();
            }
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}
