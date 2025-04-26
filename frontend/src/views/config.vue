<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import NetworkApSection from '@/widgets/network-ap.section.vue'
import SelfApSection from '@/widgets/self-ap.section.vue'
import UploadUpdateSection from '@/widgets/upload_update.section.vue'
import TelegramSettingsSection from '@/widgets/telegram-settings.section.vue'
import ContentSection from '@/components/content-section.vue'
</script>

<template>
  <main>
    <PageHeader title="/config" subtitle="home" />
    <div class="flex-auto flex flex-col gap-6 p-4 rounded-xl rounded-b-none bg-base-100">
      <TelegramSettingsSection />
      <ContentSection id="wi-fi" title="Wi-Fi">
        <div class="flex flex-col gap-2">
          <NetworkApSection />
          <SelfApSection />
        </div>
      </ContentSection>
      <ContentSection id="update" title="Update">
        <UploadUpdateSection />
      </ContentSection>
    </div>
  </main>
</template>
