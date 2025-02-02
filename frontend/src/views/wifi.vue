<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import ContentSection from '@/components/content-section.vue'
import List from '@/components/list.vue'
import Accordion from '@/components/accordion.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import { useWifiList } from '@/data/wifi'
import { computed, ref } from 'vue'

const { data } = useWifiList()

const listAdopted = computed(() => {
  const items = data.value?.map((item, index) => ({ slot: 'wifi', key: index.toString(), ...item }))
  items?.push({ slot: 'new', key: data.value?.length.toString() || '0' })
  items?.push({ slot: 'ap', key: ((data.value?.length || 0) + 1).toString() || '1' })
  return items
})
</script>

<template>
  <PageHeader />
  <ContentSection
    title="Saved networks"
    subtitle="Networks are listed in order of connection priority"
  >
    <List :items="listAdopted">
      <template #wifi="{ key, ssid, password, isFocused }">
        <Accordion v-model:open="ref(isFocused).value" class="list__entity">
          <template #summary>
            <p>{{ `${Number(key) + 1}.  ${ssid}` }}</p>
          </template>
          <template #details>
            <WifiCredentialsForm delitable :default-values="{ ssid, password }" />
          </template>
        </Accordion>
      </template>
      <template #new="{ isFocused }">
        <Accordion v-model:open="ref(isFocused).value" class="list__entity">
          <template #summary>
            <p>Save new network</p>
          </template>
          <template #details>
            <WifiCredentialsForm />
          </template>
        </Accordion>
      </template>
    </List>
  </ContentSection>
  <ContentSection title="Access point" subtitle="Edit credentials of self access point">
    <WifiCredentialsForm />
  </ContentSection>
</template>

<style scoped>
.list__entity {
  padding: 1rem;
}
</style>
