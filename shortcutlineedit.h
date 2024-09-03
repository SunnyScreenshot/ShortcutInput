#ifndef SHORTCUTLINEEDIT_H
#define SHORTCUTLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QKeySequence>

class ShortcutLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ShortcutLineEdit(QWidget *parent = nullptr);
    explicit ShortcutLineEdit(const QKeySequence &keySequence, QWidget *parent = nullptr);
    virtual ~ShortcutLineEdit() = default;

    void setKeySequence(const QKeySequence &keySequence);
    QKeySequence keySequence() const;

signals:
    void keySequenceChanged(const QKeySequence &keySequence);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void updateText();
    QKeySequence m_keySequence;
};

#endif // SHORTCUTLINEEDIT_H
