<script setup lang="ts">
import type { PropType } from 'vue'

defineProps({
  square: {
    type: Boolean,
    default: false,
  },
  variant: {
    type: String as PropType<'default' | 'danger' | 'success'>,
    default: 'default',
  },
})
</script>

<template>
  <button class="button" :class="{ 'button--square': square, [`button--${variant}`]: variant }">
    <slot />
  </button>
</template>

<style lang="css" scoped>
.button {
  width: 100%;
  font-size: 1em;
  padding: 15px 12px;
  text-align: start;
  display: flex;
  align-items: center;
  background-color: var(--palette-contrast);
  color: var(--palette-main);
  font-weight: 600;
  border: none;
  text-transform: capitalize;
}

.button--square {
  width: auto;
  padding: 0.8rem;
  text-align: center;
}

.button--danger {
  background-color: var(--palette-danger);
}

.button--success {
  background-color: var(--palette-success);
}
</style>
