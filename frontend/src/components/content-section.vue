<script setup lang="ts">
defineProps({
  title: String,
  subtitle: String,
})
</script>

<template>
  <section class="root">
    <h3 v-if="title" class="title">{{ title }}</h3>
    <p v-if="subtitle" class="subtitle">{{ subtitle }}</p>
    <slot />
  </section>
</template>

<style scoped>
.root {
  padding: 1rem 0;
  padding-inline: 1rem;
}

.title {
  font-size: 1.25rem;
  font-weight: 400;
  margin-bottom: 0.4rem;
}

.subtitle {
  font-size: 0.9rem;
  font-weight: 100;
}
</style>
