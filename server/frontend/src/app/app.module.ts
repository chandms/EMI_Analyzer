import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SweepComponent } from './sweep/sweep.component';
import { SweepService } from './sweep/sweep.service';

@NgModule({
  declarations: [
    AppComponent,
    SweepComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule
  ],
  providers: [
    SweepService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
