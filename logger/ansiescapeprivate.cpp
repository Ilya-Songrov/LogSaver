#include "ansiescapeprivate.h"

QByteArray                  AnsiEscapePrivate::postfix          { "\033[0m" };
QChar                       AnsiEscapePrivate::escape           { '\x1b' }; // \x1b == \033
QChar                       AnsiEscapePrivate::squareBracket    { '[' };
QChar                       AnsiEscapePrivate::semicolon        { ';' };
QChar                       AnsiEscapePrivate::colorTerminator  { 'm' };


QByteArray AnsiEscapePrivate::getColorsString(QString color, const GraphicsType type)
{
    // See http://en.wikipedia.org/wiki/ANSI_escape_code#Colors
    // RGB set with format: 38;2;<r>;<g>;<b>
    // ESC[ 38;2;⟨r⟩;⟨g⟩;⟨b⟩ m Select RGB foreground color
    // ESC[ 48;2;⟨r⟩;⟨g⟩;⟨b⟩ m Select RGB background color

    QByteArray result;
    if(!color.isEmpty()){
        QByteArray colorBytes;
        if(type == GraphicsType::Foreground){
            colorBytes += "38;2;";
        }
        else{
            colorBytes += "48;2;";
        }
        int r { 0 };
        int g { 0 };
        int b { 0 };
        color.remove('#');
        sscanf(color.toUtf8().constData(), "%02x%02x%02x", &r, &g, &b);

        colorBytes.append(QString::number(r).toLatin1() + ";");
        colorBytes.append(QString::number(g).toLatin1() + ";");
        colorBytes.append(QString::number(b).toLatin1() + ";");
        result.append(colorBytes);
    }
    return result;
}

QByteArray AnsiEscapePrivate::getEffectsString(const EffectsLog &effectsLog)
{
    QByteArray result;
    if(effectsLog == EffectsLog::ALL_ATTRIBUTES_OFF){
        return result;
    }
    int enumNumber = 1;
    for (int var = enumNumber; var < COUNT_EFFECTSLOG; ++var) {
        if(effectsLog & enumNumber){
            result.append(QString::number(var).toLatin1() + ";");
        }
        enumNumber *= 2;
    }
    return result;
}

QByteArray AnsiEscapePrivate::getPrefixGraphics(const QString &foregroundColor, const QString &backgroundColor, const EffectsLog &effectsLog)
{
    if (foregroundColor.isEmpty() && backgroundColor.isEmpty() && effectsLog == DEFAULT) {
        return {};
    }
    QByteArray prefix;

    QByteArray bytePrefix;
    bytePrefix.append(AnsiEscapePrivate::escape.toLatin1());
    bytePrefix.append(AnsiEscapePrivate::squareBracket.toLatin1());
    prefix.append(bytePrefix);
    prefix.append(AnsiEscapePrivate::getColorsString(foregroundColor, AnsiEscapePrivate::Foreground));
    prefix.append(AnsiEscapePrivate::getColorsString(backgroundColor, AnsiEscapePrivate::Background));
    prefix.append(AnsiEscapePrivate::getEffectsString(effectsLog));
    if(prefix.back() == AnsiEscapePrivate::semicolon.toLatin1()){
        prefix.chop(1); // remove semicolon
    }
    prefix.append(AnsiEscapePrivate::colorTerminator.toLatin1());

    const QByteArray notUsingAnsiEscapeColor { bytePrefix + AnsiEscapePrivate::colorTerminator.toLatin1() };
    if (prefix == notUsingAnsiEscapeColor) { // if the prefix is not affect, then we clear it
        prefix.clear();
    }
    return prefix;
}
