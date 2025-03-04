<script setup lang="ts">
import { defineAsyncComponent, ref, type PropType } from 'vue'
import Input from './input.vue'
import Button from './button.vue'
import type { WifiCredentials } from '@/data/config_api'
const IconClose = defineAsyncComponent(() => import('@/assets/icons/close.svg'))

const props = defineProps({
  delitable: {
    type: Boolean,
    default: false,
  },
  defaultValues: {
    type: Object as PropType<WifiCredentials>,
    default: () => ({}),
  },
})

const ssid = ref(props.defaultValues.ssid)
const password = ref(props.defaultValues.password || '')

const emit = defineEmits<{
  delete: []
  save: [value: WifiCredentials]
}>()

const handleDelete = () => {
  emit('delete')
}

const hadndleSave = () => {
  emit('save', { ssid: ssid.value, password: password.value })
}
</script>

<template>
  <form class="wifi-credentials-form" @click.stop="">
    <Input placeholder="SSID" type="text" name="ssid" v-model="ssid" />
    <Input placeholder="Password" type="password" name="password" v-model="password" />
    <div class="actions">
      <Button v-if="delitable" type="button" @click="handleDelete" square>
        <IconClose width="1.5em" />
      </Button>
      <Button type="button" @click="hadndleSave">Save</Button>
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
  gap: 0.6rem;
}
</style>
