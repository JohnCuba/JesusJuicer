<script lang="ts" setup>
defineProps({
  urlPath: String,
  label: String,
})
</script>

<template>
  <div class="relative card card-border overflow-hidden bg-base-100 shadow-xl">
    <div class="card-body">
      <h2 class="card-title font-sans">{{ label }}</h2>
      <slot />
      <router-link v-if="urlPath" :to="urlPath" class="absolute inset-0" />
    </div>
  </div>
</template>
