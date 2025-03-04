<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import InputFile from '@/components/input_file.vue'
import { useUploadFw, useUploadFs } from '@/data/update'

const { upload: uploadFw, progress: fwUploadProgress } = useUploadFw()
const { upload: uploadFs, progress: fsUploadProgress } = useUploadFs()
</script>

<template>
  <ContentSection title="Update by files" class="section">
    <InputFile accept=".bin" placeholder="system" @send="uploadFw" :progress="fwUploadProgress" />
    <InputFile accept=".bin" placeholder="flash" @send="uploadFs" :progress="fsUploadProgress" />
  </ContentSection>
</template>

<style scoped>
.section {
  display: flex;
  flex-direction: column;
  row-gap: 1em;
}
</style>
