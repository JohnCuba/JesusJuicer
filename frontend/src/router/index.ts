import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: () => import('@/views/home.vue'),
    },
    {
      path: '/wifi',
      name: 'wi-fi',
      component: () => import('@/views/wifi.vue'),
    },
    {
      path: '/update',
      name: 'update',
      component: () => import('@/views/update.vue'),
    },
  ],
})

export default router
