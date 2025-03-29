<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import InputFile from '@/components/input_file.vue'
import { useUploadFw, useUploadFs } from '@/data/update'

const { upload: uploadFw, progress: fwUploadProgress } = useUploadFw()
const { upload: uploadFs, progress: fsUploadProgress } = useUploadFs()
</script>

<template>
  <ContentSection title="Update by files">
    <div class="flex flex-col gap-4">
      <InputFile
        accept=".bin"
        placeholder="Pick system"
        class="join-item"
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
  </ContentSection>
</template>
