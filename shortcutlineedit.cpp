#include "shortcutlineedit.h"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QDebug>

ShortcutLineEdit::ShortcutLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setAlignment(Qt::AlignCenter);  // Center-align text
    setReadOnly(true);              // Make the line edit read-only
}

ShortcutLineEdit::ShortcutLineEdit(const QKeySequence &keySequence, QWidget *parent)
    : QLineEdit(parent), m_keySequence(keySequence)
{
    setAlignment(Qt::AlignCenter);  // Center-align text
    setReadOnly(true);              // Make the line edit read-only
    updateText();
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

void ShortcutLineEdit::keyPressEvent(QKeyEvent *event)
{
    // Handle modifier keys and regular keys separately
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        setKeySequence(QKeySequence()); // Clear key sequence
    // } else if (event->modifiers() == Qt::NoModifier) {
    //     // Handle keys with no modifier
    //     QKeySequence newSequence(event->key());
    //     setKeySequence(newSequence);
    } else {
        // Handle keys with modifiers
        // event->key()

        if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control || event->key() == Qt::Key_Alt) {
            // Do not update key sequence for modifier keys only
            return;
        }

        QKeySequence newSequence(event->key() | event->modifiers());
        setKeySequence(newSequence);
    }

    // Prevent the default behavior of QLineEdit
    event->ignore();
}

void ShortcutLineEdit::keyReleaseEvent(QKeyEvent *event)
{
    // Optional: Handle key release events if needed
    event->ignore(); // Ignore to prevent default behavior
}

void ShortcutLineEdit::updateText()
{
    if (m_keySequence.isEmpty()) {
        setText(QString());
    } else {
        setText(m_keySequence.toString(QKeySequence::NativeText));
    }
}
