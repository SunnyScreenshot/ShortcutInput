#include "XKeySequenceEdit.h"
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <qevent.h>
#include <windows.h> // For LowLevelKeyboardProc

HHOOK XKeySequenceEdit::hook = nullptr;
XKeySequenceEdit* XKeySequenceEdit::instance = nullptr;

XKeySequenceEdit::XKeySequenceEdit(QWidget *parent)
    : QKeySequenceEdit(parent)
{
    initUI();
}

XKeySequenceEdit::XKeySequenceEdit(const QKeySequence &keySequence, QWidget *parent)
    : QKeySequenceEdit(keySequence, parent)
{
    initUI();
}

XKeySequenceEdit::~XKeySequenceEdit()
{
    if (hook) {
        UnhookWindowsHookEx(hook);
    }
    instance = nullptr; // Clear the static instance pointer
}

void XKeySequenceEdit::initUI()
{
    instance = this; // Set the static instance pointer
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

    // Connect signal to the slot
    // connect(this, &XKeySequenceEdit::printScreenDetected, this, &XKeySequenceEdit::handlePrintScreenSlot);
}

LRESULT CALLBACK XKeySequenceEdit::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
        if (wParam == WM_KEYDOWN && pKeyboard->vkCode == VK_SNAPSHOT) {
            if (instance) {
                instance->handlePrintScreenSlot();
            }
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}

void XKeySequenceEdit::handlePrintScreen()
{
    QKeySequence keySequence(Qt::Key_Print);
    setKeySequence(keySequence);
    emit sigKeySeqChanged(keySequence);
}

// Slot implementation
void XKeySequenceEdit::handlePrintScreenSlot()
{
    handlePrintScreen();
}

void XKeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyPressEvent(event);

    if (event->key() == Qt::Key_Print) {
        handlePrintScreen();
        return;
    }
}
