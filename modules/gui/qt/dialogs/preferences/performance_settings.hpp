/****************************************************************************
 * SPDX-License-Identifier: GPL-2.0-or-later
 ****************************************************************************/
#ifndef VLC_QT_PREFERENCES_PERFORMANCE_SETTINGS_HPP
#define VLC_QT_PREFERENCES_PERFORMANCE_SETTINGS_HPP

#include <QHash>
#include <QString>

namespace PerformanceSettings {
struct VideoPipeline {
    QString output, decoder, upscaling, filters;
};

inline bool rtxVsrEnabled(const VideoPipeline &)
{
    return false;
}

inline QString postProcessingFilters(const QString &filters, bool)
{
    return filters;
}

inline QHash<QString, QString> rtxVsrChanges(const VideoPipeline &, bool)
{
    return {};
}
}
#endif
