<script setup lang="ts">
const model = defineModel()
</script>

<template>
  <input class="input" v-model="model" />
</template>

<style scoped>
.input {
  position: relative;
  font-size: 1em;
  width: 100%;
  color: var(--palette-contrast);
  background-color: transparent;
  border: 1px solid var(--palette-main);
  padding: 14px 12px;
}
</style>
