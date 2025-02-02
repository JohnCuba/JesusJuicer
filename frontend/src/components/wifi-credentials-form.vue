<script setup lang="ts">
import { defineAsyncComponent, type PropType } from 'vue'
import Input from './input.vue'
import Button from './button.vue'
import type { WifiCredentials } from '@/data/config_api'
const IconClose = defineAsyncComponent(() => import('@/assets/icons/close.svg'))

defineProps({
  delitable: {
    type: Boolean,
    default: false,
  },
  defaultValues: {
    type: Object as PropType<WifiCredentials>,
    default: () => ({}),
  },
})

const emit = defineEmits<{
  delete: []
  save: [value: WifiCredentials]
}>()

const handleDelete = () => {
  emit('delete')
}

const hadndleSave = () => {}
</script>

<template>
  <form class="wifi-credentials-form">
    <Input placeholder="SSID" type="text" :value="defaultValues.ssid" />
    <Input placeholder="Password" type="password" :value="defaultValues.password" />
    <div class="actions">
      <Button v-if="delitable" @click="handleDelete" square>
        <IconClose />
      </Button>
      <Button @click="hadndleSave">Save</Button>
    </div>
  </form>
</template>

<style scoped>
.wifi-credentials-form {
  display: flex;
  flex-direction: column;
  gap: 1rem;
  padding: 1rem 0;
}

.actions {
  display: flex;
  flex-direction: row;
  gap: 0.2rem;
}
</style>
