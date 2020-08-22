QDesktopService to open file or folder containing file...
http://doc.qt.digia.com/qt/dialogs-findfiles.html


Subclassing QSpinBox

If using prefix(), suffix(), and specialValueText() don't provide enough control, you subclass QSpinBox and reimplement valueFromText() and textFromValue(). For example, here's the code for a custom spin box that allows the user to enter icon sizes (e.g., "32 x 32"):

 int IconSizeSpinBox::valueFromText(const QString &text) const
 {
     QRegExp regExp(tr("(\\d+)(\\s*[xx]\\s*\\d+)?"));

     if (regExp.exactMatch(text)) {
         return regExp.cap(1).toInt();
     } else {
         return 0;
     }
 }

 QString IconSizeSpinBox::textFromValue(int value) const
 {
     return tr("%1 x %1").arg(value);
 }
See the Icons example for the full source code.
