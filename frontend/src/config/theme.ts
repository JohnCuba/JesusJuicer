import { computed, onMounted, ref, watch } from 'vue'

export const COLOR_THEMES = {
  light: 'lemonade',
  dark: 'forest',
  auto: 'auto',
} as const

type COLOR_THEMES_KEYS = keyof typeof COLOR_THEMES

const STORAGE_KEY = 'color-theme'
const ATTRIBUTE_KEY = 'data-theme'

const getSavedColorTheme = () => {
  return (localStorage.getItem(STORAGE_KEY) || 'auto') as COLOR_THEMES_KEYS
}

const saveColorTheme = (value: COLOR_THEMES_KEYS) => {
  localStorage.setItem(STORAGE_KEY, value)
}

export const initSavedColorTheme = () => {
  const colorTheme = getSavedColorTheme()
  setColorTheme(colorTheme)
}

const setColorTheme = (value: COLOR_THEMES_KEYS) => {
  document.documentElement.setAttribute(ATTRIBUTE_KEY, COLOR_THEMES[value])
}

const currentColorTheme = ref(getSavedColorTheme())

export const useColorThemeWatcher = () => {
  const handleSavedThemeChange = (event: StorageEvent) => {
    if (event.key !== STORAGE_KEY) return

    currentColorTheme.value = event.newValue as COLOR_THEMES_KEYS
  }

  onMounted(() => {
    window.addEventListener('storage', handleSavedThemeChange)
    return () => {
      window.removeEventListener('storage', handleSavedThemeChange)
    }
  })

  watch(currentColorTheme, setColorTheme)
}

export const useColorTheme = () => {
  const changeTheme = (value: COLOR_THEMES_KEYS) => {
    saveColorTheme(value)
    window.dispatchEvent(new StorageEvent('storage', { key: STORAGE_KEY, newValue: value }))
  }
  return { currentTheme: computed(() => currentColorTheme.value), changeTheme }
}
