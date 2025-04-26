<script setup lang="ts">
import { defineAsyncComponent, ref, watch, type PropType } from 'vue'
import type { WifiCredentials } from '@/data/config_api'
const IconClose = defineAsyncComponent(() => import('@/assets/icons/close.svg'))

const props = defineProps({
  delitable: Boolean,
  defaultValues: {
    type: Object as PropType<WifiCredentials>,
    default: () => ({}),
  },
  isLoading: Boolean,
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

watch(
  () => props.isLoading,
  () => {
    ssid.value = props.defaultValues.ssid
    password.value = props.defaultValues.password || ''
  },
)
</script>

<template>
  <form class="flex flex-col gap-2" @click.stop="">
    <input
      placeholder="SSID"
      type="text"
      name="ssid"
      class="input w-full"
      :class="{
        ['skeleton rounded-field']: isLoading,
      }"
      :disabled="isLoading"
      v-model="ssid"
    />
    <input
      placeholder="Password"
      type="password"
      name="password"
      class="input w-full"
      :class="{
        ['skeleton rounded-field']: isLoading,
      }"
      :disabled="isLoading"
      v-model="password"
    />
    <div class="flex justify-end gap-4">
      <button
        v-if="delitable"
        type="button"
        @click="handleDelete"
        class="btn btn-warning btn-square"
      >
        <IconClose width="1.5em" />
      </button>
      <button type="button" @click="hadndleSave" class="btn btn-success">save</button>
    </div>
  </form>
</template>
