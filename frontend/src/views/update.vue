<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import UploadUpdateSection from '@/widgets/upload_update.section.vue'
</script>

<template>
  <PageHeader title="/update" />
  <div class="flex-auto flex flex-col gap-4 p-4 rounded-xl rounded-b-none bg-base-100">
    <UploadUpdateSection />
  </div>
</template>
