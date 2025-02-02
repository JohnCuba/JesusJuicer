<script lang="ts" setup>
import { defineAsyncComponent, ref, watch } from 'vue'
const IconChevronDown = defineAsyncComponent(() => import('@/assets/icons/chevron-down.svg'))

const visibilityState = defineModel('open', {
  type: Boolean,
})

const localOpenState = ref(visibilityState.value)

const handleClickSummary = () => {
  localOpenState.value = !localOpenState.value
}

watch(visibilityState, (value) => {
  localOpenState.value = value
})
</script>

<template>
  <div>
    <div class="summary" @click="handleClickSummary">
      <slot name="summary" />
      <IconChevronDown class="summary__mark" :class="{ 'summary__mark--open': localOpenState }" />
    </div>
    <div v-if="localOpenState">
      <slot name="details" />
    </div>
  </div>
</template>

<style scoped>
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
