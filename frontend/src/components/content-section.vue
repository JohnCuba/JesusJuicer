<script setup lang="ts">
defineProps({
  id: String,
  title: String,
})
</script>

<template>
  <section :id="id" class="flex flex-col gap-3">
    <h2 class="text-xl">{{ title }}</h2>
    <div class="relative flex flex-col gap-2">
      <slot />
    </div>
  </section>
</template>
