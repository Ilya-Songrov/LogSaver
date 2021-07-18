#pragma once

#include <QObject>
#include <QDebug>

constexpr auto START_LINE_EFFECTSLOG = __LINE__;
enum EffectsLog{
    ALL_ATTRIBUTES_OFF          = 0x0000,
    DEFAULT                     = ALL_ATTRIBUTES_OFF,
    BOLD                        = 0x0001,
    FADED                       = 0x0002,
    ITALICS                     = 0x0004,
    UNDERSCORE                  = 0x0008,
    BLINK_ON                    = 0x0010,
    BLINKING_SLOW               = BLINK_ON,
    BLINKING_FAST               = 0x0020,
    REVERSE_VIDEO_ON            = 0x0040,
    SWAP_BACKGROUND_FOREGROUND  = REVERSE_VIDEO_ON,
    CONCEALED_ON                = 0x0080, // Not supported everywhere
    CROSSED_OUT                 = 0x0100,
};
constexpr auto COUNT_EFFECTSLOG = __LINE__ - START_LINE_EFFECTSLOG - 3;


class AnsiEscapePrivate{
public:
    enum GraphicsType { Foreground, Background, };
    static QByteArray postfix       ;
    static QChar escape             ;
    static QChar squareBracket      ;
    static QChar semicolon          ;
    static QChar colorTerminator    ;

    static QByteArray getColorsString(QString color, const GraphicsType type);
    static QByteArray getEffectsString(const EffectsLog &effectsLog);
    static QByteArray getPrefixGraphics(const QString &foregroundColor = "",
                                        const QString &backgroundColor = "",
                                        const EffectsLog &effectsLog = DEFAULT);
};
