<script setup lang="ts">
defineProps({
  title: {
    type: String,
    default: 'Jesus juicer',
  },
})
</script>

<template>
  <header class="flex flex-col-reverse min-h-60 p-4 bg-neutral">
    <div v-if="$slots.default" class="flex-auto">
      <slot />
    </div>
    <h1 class="font-sans text-4xl text-base-100">{{ title }}</h1>
    <RouterLink to="/" class="font-sans text-lg text-base-300">home</RouterLink>
  </header>
</template>
