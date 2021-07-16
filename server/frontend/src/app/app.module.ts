import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { RouterModule } from '@angular/router';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SweepComponent } from './sweep/sweep.component';
import { SweepService } from './service/sweep.service';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { DeviceComponent } from './device/device.component';
import { DeviceService } from './service/device.service';
import { LocationComponent } from './location/location.component';
import { MapAllComponent } from './map-all/map-all.component';

@NgModule({
  declarations: [
    AppComponent,
    SweepComponent,
    DeviceComponent,
    LocationComponent,
    MapAllComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
    HttpClientModule,
    BrowserAnimationsModule,
    RouterModule.forRoot([
      {
        path: '',
        component: SweepComponent
      },
      {
        path: 'device/:device_name',
        component: DeviceComponent
      },
      {
        path: 'location/:device_name',
        component: LocationComponent
      }
    ])
  ],
  providers: [
    SweepService,
    DeviceService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
