import { createApp } from 'vue'
import App from './app.vue'
import router from './router'
import { initSavedColorTheme } from './config/theme'

initSavedColorTheme()

const app = createApp(App)

app.use(router)

app.mount('#app')
