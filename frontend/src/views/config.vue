<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import NetworkApSection from '@/widgets/network-ap.section.vue'
import SelfApSection from '@/widgets/self-ap.section.vue'
import UploadUpdateSection from '@/widgets/upload_update.section.vue'
import TelegramSettingsSection from '@/widgets/telegram-settings.section.vue'
</script>

<template>
  <main>
    <PageHeader title="/config" subtitle="home" />
    <div class="flex-auto flex flex-col gap-4 p-4 rounded-xl rounded-b-none bg-base-100">
      <TelegramSettingsSection />
      <h2 id="wi-fi" class="mt-4 text-xl">Wi-Fi</h2>
      <NetworkApSection />
      <SelfApSection />
      <h2 id="update" class="mt-4 text-xl">Update</h2>
      <UploadUpdateSection />
    </div>
  </main>
</template>
