<script setup lang="ts">
import { useColorThemeWatcher } from './config/theme'

useColorThemeWatcher()
</script>

<template>
  <div class="bg-neutral">
    <div class="min-h-dvh max-w-lg mx-auto flex flex-col">
      <RouterView />
    </div>
  </div>
</template>

<style lang="css">
@import url('https://fonts.googleapis.com/css2?family=Comfortaa:wght@300..700&display=swap');

@import 'tailwindcss';
@plugin "daisyui" {
  themes:
    lemonade --default,
    forest --prefersdark;
}
@plugin "daisyui/theme" {
  name: 'lemonade';
  default: true;
  --color-neutral: #706d54;
}

:root {
  --cardtitle-fs: var(--text-2xl);

  --font-sans:
    'Comfortaa', ui-sans-serif, system-ui, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji',
    'Segoe UI Symbol', 'Noto Color Emoji';
}
</style>
