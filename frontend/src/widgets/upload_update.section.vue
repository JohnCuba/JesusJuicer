<script setup lang="ts">
import InputFile from '@/components/input_file.vue'
import { useUploadFw, useUploadFs } from '@/data/update'

const { upload: uploadFw, progress: fwUploadProgress } = useUploadFw()
const { upload: uploadFs, progress: fsUploadProgress } = useUploadFs()
</script>

<template>
  <div class="flex flex-col gap-1">
    <h3>by files</h3>
    <InputFile
      accept=".bin"
      placeholder="Pick system"
      @send="uploadFw"
      :progress="fwUploadProgress"
    />
    <InputFile
      accept=".bin"
      placeholder="Pick flash"
      @send="uploadFs"
      :progress="fsUploadProgress"
    />
  </div>
</template>
