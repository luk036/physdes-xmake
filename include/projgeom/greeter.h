#pragma once

#include <string>

namespace projgeom {

    /**  Language codes to be used with the ProjGeom class */
    enum class LanguageCode { EN, DE, ES, FR };

    /**
     * @brief A class for saying hello in multiple languages
     */
    class ProjGeom {
        std::string name;

      public:
        /**
         * @brief Creates a new projgeom
         * @param[in] name the name to greet
         */
        ProjGeom(std::string name);

        /**
         * @brief Creates a localized string containing the greeting
         * @param[in] lang the language to greet in
         * @return a string containing the greeting
         */
        std::string greet(LanguageCode lang = LanguageCode::EN) const;
    };

}  // namespace projgeom
