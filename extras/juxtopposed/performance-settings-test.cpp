/****************************************************************************
 * SPDX-License-Identifier: GPL-2.0-or-later
 ****************************************************************************/
#include "modules/gui/qt/dialogs/preferences/performance_settings.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #condition); ++failures; } } while (0)

int main()
{
    using namespace PerformanceSettings;
    const VideoPipeline defaults{QStringLiteral("any"), QStringLiteral("any"),
                                 QStringLiteral("linear"), QStringLiteral("postproc")};
    CHECK(!rtxVsrEnabled(defaults));

    const QHash<QString, QString> wantedOn{
        {QStringLiteral("vout"), QStringLiteral("direct3d11")},
        {QStringLiteral("dec-dev"), QStringLiteral("d3d11_filters")},
        {QStringLiteral("d3d11-upscale-mode"), QStringLiteral("super")},
        {QStringLiteral("video-filter"), QStringLiteral("adjust:sharpen")}};
    const VideoPipeline previous{QStringLiteral("direct3d9"), QStringLiteral("nvdec"),
                                 QStringLiteral("point"), QStringLiteral("adjust:postproc:sharpen:postproc")};
    CHECK(rtxVsrChanges(previous, true) == wantedOn);

    const QHash<QString, QString> wantedOff{
        {QStringLiteral("d3d11-upscale-mode"), QStringLiteral("linear")}};
    CHECK(rtxVsrChanges(previous, false) == wantedOff);

    const VideoPipeline compatible{QStringLiteral("direct3d11"), QStringLiteral("d3d11_filters"),
                                   QStringLiteral("super"), QStringLiteral("adjust:sharpen")};
    CHECK(rtxVsrEnabled(compatible));
    for (int field = 0; field < 4; ++field) {
        auto invalid = compatible;
        if (field == 0) invalid.output = QStringLiteral("gl");
        if (field == 1) invalid.decoder = QStringLiteral("nvdec");
        if (field == 2) invalid.upscaling = QStringLiteral("linear");
        if (field == 3) invalid.filters = QStringLiteral("adjust:postproc:sharpen");
        CHECK(!rtxVsrEnabled(invalid));
    }
    auto emptyFilters = compatible;
    emptyFilters.filters.clear();
    CHECK(rtxVsrEnabled(emptyFilters));

    CHECK(postProcessingFilters(QStringLiteral("adjust:postproc:sharpen:postproc"), false)
          == QStringLiteral("adjust:sharpen"));
    CHECK(postProcessingFilters(QStringLiteral("adjust:postproc:sharpen:postproc"), true)
          == QStringLiteral("adjust:sharpen:postproc"));
    CHECK(postProcessingFilters(QString(), false).isEmpty());
    CHECK(postProcessingFilters(QString(), true) == QStringLiteral("postproc"));
    CHECK(postProcessingFilters(QStringLiteral(":adjust::postproc:"), false) == QStringLiteral("adjust"));
    CHECK(postProcessingFilters(QStringLiteral("my-postproc:adjust"), false)
          == QStringLiteral("my-postproc:adjust"));
    std::printf("Performance settings: %d failure(s)\n", failures);
    return failures ? 1 : 0;
}
