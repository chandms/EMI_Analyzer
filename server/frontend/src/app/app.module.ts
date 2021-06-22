import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { RouterModule } from '@angular/router';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SweepComponent } from './sweep/sweep.component';
import { SweepService } from './sweep/sweep.service';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { DeviceComponent } from './device/device.component';

@NgModule({
  declarations: [
    AppComponent,
    SweepComponent,
    DeviceComponent
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
        path: 'device',
        component: DeviceComponent
      }
    ])
  ],
  providers: [
    SweepService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
