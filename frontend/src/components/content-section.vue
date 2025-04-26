<script setup lang="ts">
defineProps({
  id: String,
  title: String,
})
</script>

<template>
  <section :id="id" class="flex flex-col gap-3">
    <h2 class="text-xl">{{ title }}</h2>
    <slot />
  </section>
</template>
