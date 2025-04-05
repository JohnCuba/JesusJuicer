<script setup lang="ts">
import { useColorTheme } from '@/config/theme'
import { defineAsyncComponent } from 'vue'

const IconLight = defineAsyncComponent(() => import('@/assets/icons/light.svg'))
const IconDark = defineAsyncComponent(() => import('@/assets/icons/dark.svg'))
const IconAuto = defineAsyncComponent(() => import('@/assets/icons/contrast.svg'))

const { changeTheme, currentTheme } = useColorTheme()
</script>

<template>
  <div class="cursor-pointer">
    <IconLight v-if="currentTheme === 'light'" @click="changeTheme('dark')" />
    <IconDark v-if="currentTheme === 'dark'" @click="changeTheme('auto')" />
    <IconAuto v-if="currentTheme === 'auto'" @click="changeTheme('light')" />
  </div>
</template>
