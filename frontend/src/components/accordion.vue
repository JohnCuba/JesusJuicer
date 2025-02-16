<script lang="ts" setup>
import { defineAsyncComponent } from 'vue'
const IconChevronDown = defineAsyncComponent(() => import('@/assets/icons/chevron-down.svg'))

const isOpen = defineModel<boolean>()

const handleClickSummary = () => {
  isOpen.value = !isOpen.value
}
</script>

<template>
  <div class="accordion">
    <div class="summary" @click="handleClickSummary">
      <slot name="summary" />
      <IconChevronDown class="summary__mark" :class="{ 'summary__mark--open': isOpen }" />
    </div>
    <div v-if="isOpen">
      <slot name="details" />
    </div>
  </div>
</template>

<style scoped>
.accordion {
  padding: 1rem;
}

.summary {
  display: flex;
  justify-content: space-between;
  align-items: center;
  outline: 0;
  outline-style: none;
}

.summary__mark {
  transition: transform 0.2s;
  transform: rotate(0deg);
}

.summary__mark--open {
  transform: rotate(180deg);
}
</style>
