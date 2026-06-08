#ifndef SENECA_SETTINGS_H
#define SENECA_SETTINGS_H

namespace seneca {

    struct Settings {
        unsigned short m_maxSummaryWidth{ 80 };
        bool           m_tableView{ false };
    };

    // Global settings instance declaration.
    extern Settings g_settings;

} // namespace seneca

#endif // SENECA_SETTINGS_H
