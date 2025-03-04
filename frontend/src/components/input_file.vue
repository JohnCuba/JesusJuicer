<script setup lang="ts">
import { computed, defineAsyncComponent, ref, watch } from 'vue'
import Button from '@/components/button.vue'

const IconCancel = defineAsyncComponent(() => import('@/assets/icons/close.svg'))

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
const mainButtonText = computed(() =>
  model.value ? 'upload ' + model.value?.name : props.placeholder,
)
const percentProgress = computed(() => (props.progress || 0) * 100 + '%')

const handleInput = async (event: Event) => {
  if (
    (event.target as HTMLInputElement).files &&
    (event.target as HTMLInputElement)?.files?.length
  ) {
    model.value = (event.target as HTMLInputElement)?.files?.item(0)
  }
}

const handleClickCancel = () => {
  model.value = null
  if (controlRef.value) controlRef.value.value = ''
}

const handleClicAction = () => {
  if (!model.value) {
    controlRef.value?.click()
  } else {
    emit('send', model.value)
  }
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
  <div class="file_input">
    <input
      class="file_input__control"
      type="file"
      ref="controlRef"
      @change="handleInput"
      v-bind="$attrs"
    />
    <Button v-if="model" type="reset" square variant="danger" @click="handleClickCancel">
      <IconCancel width="1.5em" />
    </Button>
    <Button
      type="button"
      class="file_input__main_button main_button"
      :progress="progress"
      @click="handleClicAction"
    >
      <span class="main_button__content">
        {{ mainButtonText }}
      </span>
    </Button>
  </div>
</template>

<style scoped>
.file_input {
  position: relative;
  display: flex;
  gap: 0.6rem;
}

.file_input__control {
  display: none;
}

.main_button__content {
  position: relative;
  text-transform: none;
}

.main_button::before {
  content: '';
  position: absolute;
  top: 0;
  right: 0;
  bottom: 0;
  left: 0;
  height: 100%;
  width: v-bind(percentProgress);
  background-color: var(--palette-success);
  opacity: v-bind(percentProgress);

  transition: width 0.2s ease-out;
}
</style>
