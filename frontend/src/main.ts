import { createApp } from 'vue'
import App from './app.vue'
import router from './router'

import '@/assets/theme-default.css'
import '@/assets/base.css'

const app = createApp(App)

app.use(router)

app.mount('#app')
