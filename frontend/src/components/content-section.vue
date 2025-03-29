<script setup lang="ts">
defineProps({
  title: String,
  subtitle: String,
})
</script>

<template>
  <section>
    <div class="mb-4">
      <h3 v-if="title" class="text-xl">{{ title }}</h3>
      <p v-if="subtitle">{{ subtitle }}</p>
    </div>
    <slot />
  </section>
</template>
