import i18n from "i18next";
import { initReactI18next } from "react-i18next";

import en from '../translations/en.json';
import ar from '../translations/ar.json';
import he from '../translations/he.json';


i18n
.use(initReactI18next)
.init({
  compatibilityJSON: 'v3',
  resources: {
    EN: {
      translation: en,
    },
    AR: {
      translation: ar,
    },
    HE:{
      translation: he,
    }
  },
  lng: 'AR', // Set the default language here
  fallbackLng: 'AR', // Fallback language if translation for a specific key is not available
  interpolation: {
    escapeValue: false,
  },
});
export default i18n;

