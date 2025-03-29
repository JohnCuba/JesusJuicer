<script setup lang="ts">
import { ref, watch } from 'vue'

defineOptions({
  inheritAttrs: false,
})

const props = defineProps({
  placeholder: String,
  progress: Number,
  accept: String,
})

const emit = defineEmits<{
  chosen: [file: File]
  send: [file: File]
}>()

const controlRef = ref<HTMLInputElement>()
const model = defineModel<File | null>()

const handleInput = async (event: Event) => {
  if ((event.target as HTMLInputElement)?.files?.length) {
    model.value = (event.target as HTMLInputElement)?.files?.item(0)
  } else {
    model.value = null
  }
}

const handleClickCancel = () => {
  model.value = null
  if (controlRef.value) controlRef.value.value = ''
}

const handleClicAction = () => {
  if (!model.value) return
  emit('send', model.value)
}

watch(
  () => props.progress,
  (current, prev) => {
    if (current === 0 && prev === 100) {
      handleClickCancel()
    }
  },
)
</script>

<template>
  <div class="flex flex-col gap-2">
    <fieldset class="fieldset w-full">
      <legend class="fieldset-legend">{{ placeholder }}</legend>
      <input
        :disabled="Boolean(progress)"
        class="file-input w-full"
        type="file"
        ref="controlRef"
        @change="handleInput"
        v-bind="$attrs"
        :class="{
          ['file-input-success']: !Boolean(progress),
        }"
      />
    </fieldset>
    <button
      class="btn w-full"
      :class="{
        ['btn-info btn-dash']: Boolean(progress),
        ['btn-success']: !Boolean(progress),
      }"
      :disabled="!Boolean(model)"
      @click="handleClicAction"
    >
      <span v-if="progress" class="loading loading-spinner"></span>
      {{ progress ? `uploading ${progress}%` : 'upload' }}
    </button>
  </div>
</template>
